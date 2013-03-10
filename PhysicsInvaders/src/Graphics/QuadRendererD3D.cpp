/*
 * This source file is part of one of Jeremy Burgess's samples.
 *
 * Copyright (c) 2013 Jeremy Burgess 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// My header
#include "QuadRendererD3D.h"

// D3D headers
#include <D3D11.h>
#include <xnamath.h>
#include <D3DX11effect.h>

// Project Headers
#include "Texture2DD3D.h"
#include "EffectD3D.h"
#include "MoveableTexturedQuad.h"
#include "EigenToD3D.h"
#include "RendererD3D.h"
#include "ICamera.h"
#include "Core/RunInformation.h"

// Vertex structure for rendering quads
struct vertex
{
	vertex(XMFLOAT3 pos, XMFLOAT4 col, XMFLOAT2 tCoord)
	{
		m_Pos = pos;
		m_Colour = col;
		m_texCoord = tCoord;
	}

	vertex() {}

	XMFLOAT3 m_Pos;
	XMFLOAT4 m_Colour;
	XMFLOAT2 m_texCoord;
};

// Constant for the name of the technique in the shader we're looking for.
const char* TEXTURED_EFFECT_NAME = "texturedBasic";

QuadRendererD3D::QuadRendererD3D(
	const RendererD3D& renderer, 
	const char* effectFilename, 
	size_t numReservedQuadSpots) :
	m_renderingEffect(nullptr)
{
	// Reserve space
	m_quadsForRendering.reserve(numReservedQuadSpots);

	// Load the effect
	m_renderingEffect = new EffectD3D();
	m_renderingEffect->LoadEffectFromFile(effectFilename, renderer);

	// Create a layout for it.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create input layout for the shader
	m_renderingEffect->CreateInputLayout( 
		TEXTURED_EFFECT_NAME, 
		layout,
		sizeof(layout) / sizeof(layout[0]),
		renderer );

	// Create a vertex buffer
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( vertex ) * 4; // This is just for quads.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	renderer.GetDevice()->CreateBuffer( &bd, NULL, &m_buffer );
}

QuadRendererD3D::~QuadRendererD3D(void)
{
	ClearRenderList();
	delete m_renderingEffect;

	if (m_buffer)
		m_buffer->Release();
}

void QuadRendererD3D::AddToRenderList(const MoveableTexturedQuad* quad)
{
	m_quadsForRendering.push_back(quad);
}

void QuadRendererD3D::ClearRenderList()
{
	m_quadsForRendering.clear();
}

void QuadRendererD3D::Render(const RendererD3D& renderer, const ICamera* camera)
{
	// Sort all quads.
	std::sort(m_quadsForRendering.begin(), m_quadsForRendering.end(), 
		[camera](const MoveableTexturedQuad* first, const MoveableTexturedQuad* second)
	    {
			return camera->GetRenderDistanceToPoint(first->GetPosition()) > camera->GetRenderDistanceToPoint(second->GetPosition());
	    });

	// Get the device
	ID3D11DeviceContext* device = renderer.GetDeviceContext();
		
	// Set vertex buffer
	UINT stride = sizeof( vertex );
	UINT offset = 0;
	device->IASetVertexBuffers( 0, 1, &m_buffer, &stride, &offset );

	// Set primitive topology 
	device->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// Set the input layout for the vertices we're about to work with.
	device->IASetInputLayout(m_renderingEffect->GetInputLayout());
	
	// Create world matrix
	XMMATRIX w = XMMatrixIdentity();
	XMMATRIX viewMatrix;
	XMMATRIX projectionMatrix;
	
	// Set up the view and projection matrices
	//*****************************************************************************
	XMFLOAT3 cameraMatrix[3] = {EigenToD3D::EigenVector3fToD3DVector3(camera->GetRenderPosition()),
								EigenToD3D::EigenVector3fToD3DVector3(camera->GetRenderFocusPosition()),
								EigenToD3D::EigenVector3fToD3DVector3(camera->GetRenderUp()) };
	viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&cameraMatrix[0]), XMLoadFloat3(&cameraMatrix[1]), XMLoadFloat3(&cameraMatrix[2]));		
	
	switch (camera->GetRenderCameraType())
	{
		case ICamera::CAMERA_TYPE_PERSPECTIVE:
			projectionMatrix = XMMatrixPerspectiveFovLH(camera->GetRenderFOVAngleY(), 
				camera->GetRenderAspectMultiplier() * RunInformation::GetScreenDimensions().GetScreenAspectRatio(), 
				camera->GetRenderNearClipDistance(), 
				camera->GetRenderFarClipDistance());
			break;
		case ICamera::CAMERA_TYPE_ORTHOGRAPHIC:
			{
				Rect<float> rect = camera->GetRenderOrthographicRect();
				projectionMatrix = XMMatrixOrthographicOffCenterLH(
					rect.m_left, rect.m_right, rect.m_bottom, rect.m_top, 
					camera->GetRenderNearClipDistance(), 
					camera->GetRenderFarClipDistance());
			}
			break;
	}

	// Set effect matrices
	ID3DX11EffectMatrixVariable* worldMatrixVariable = m_renderingEffect->GetVariable( "World" )->AsMatrix();
	ID3DX11EffectMatrixVariable* viewMatrixVariable = m_renderingEffect->GetVariable( "View" )->AsMatrix();
	ID3DX11EffectMatrixVariable* projectionMatrixVariable = m_renderingEffect->GetVariable( "Projection" )->AsMatrix();

	// These won't change per quad
	viewMatrixVariable->SetMatrix((float*)&viewMatrix);
	projectionMatrixVariable->SetMatrix((float*)&projectionMatrix);
	
	// Get technique description (required for rendering)
	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* technique = m_renderingEffect->GetTechnique(TEXTURED_EFFECT_NAME);
	technique->GetDesc( &techDesc );

	// Shared variables
	UINT numVertices = 4;
	vertex* v = NULL;

	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		// Setup the variables required by the quad, and draw it.
		for (auto qIt = m_quadsForRendering.begin(); qIt != m_quadsForRendering.end(); ++qIt)
		{			
			// Get the quad and bind its world transform.
			const MoveableTexturedQuad* quad = *qIt;
			EigenToD3D::EigenAffineToD3DMatrix(quad->GetTransform(), w);
			worldMatrixVariable->SetMatrix((float*)&w);

			// Map the vertices (their colours can change).
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			device->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			v = (vertex*)mappedResource.pData;
			XMFLOAT4 colour = EigenToD3D::EigenVector4fToD3DVector4(quad->GetColour());

			v[0] = vertex( XMFLOAT3(0,0,0),colour,XMFLOAT2(0.0f, 1.0f) );
			v[1] = vertex( XMFLOAT3(0,1,0),colour,XMFLOAT2(0.0f, 0.0f) );
			v[2] = vertex( XMFLOAT3(1,0,0),colour,XMFLOAT2(1.0f, 1.0f) );
			v[3] = vertex( XMFLOAT3(1,1,0),colour,XMFLOAT2(1.0f, 0.0f) );

			device->Unmap(m_buffer, 0);
			
			// Bind the texture
			ID3DX11EffectShaderResourceVariable* textureVar = m_renderingEffect->GetVariable( "tex2D" )->AsShaderResource();
			textureVar->SetResource( static_cast<const Texture2DD3D*>(quad->GetTexture())->GetShaderResourceViewD3D() );

			// Apply technique pass
			technique->GetPassByIndex( p )->Apply( 0, device );

			// Draw
			device->Draw( numVertices, 0 );
		}
	}
}