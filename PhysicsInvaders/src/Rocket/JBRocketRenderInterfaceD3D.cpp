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
#include "JBRocketRenderInterfaceD3D.h"

// D3D headers
#include <D3D11.h>
#include <xnamath.h>
#include <D3DX11Effect.h>

// Project headers
#include "Graphics\EffectD3D.h"
#include "Graphics\RendererD3D.h"
#include "Graphics\Texture2DD3D.h"
#include "Core\RunInformation.h"

// Vertex structure for rendering with our simple unlit shader.
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
const char* TEXTURED_EFFECT_NAME_UI = "texturedBasic";
const char* UNTEXTURED_EFFECT_NAME_UI = "untexturedBasic";

JBRocketRenderInterfaceD3D::JBRocketRenderInterfaceD3D(
		RendererD3D* renderer,
		const char* effectFilename) :
	m_renderer(renderer)
{
	// Load the effect
	m_renderingEffect = new EffectD3D();
	m_renderingEffect->LoadEffectFromFile(effectFilename, *renderer);

	// Create a layout for it.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Create input layout for the shader
	m_renderingEffect->CreateInputLayout( 
		TEXTURED_EFFECT_NAME_UI, 
		layout,
		sizeof(layout) / sizeof(layout[0]),
		*renderer );

	// Initialise the matrices in the effect that will never change (for now).
	// (We only render to ortho 2D, size of the screen for the moment)

	// Set up the view and projection matrices
	//*****************************************************************************
	XMFLOAT3 camera[3] = {	XMFLOAT3(0.0f, 0.0f, -5.0f),
								XMFLOAT3(0.0f, 0.0f, 1.0f),
								XMFLOAT3(0.0f, 1.0f, 0.0f) };
	XMMATRIX viewMatrix = XMMatrixLookAtLH(XMLoadFloat3(&camera[0]), XMLoadFloat3(&camera[1]), XMLoadFloat3(&camera[2]));		
	XMMATRIX projectionMatrix = XMMatrixOrthographicOffCenterLH(0, RunInformation::GetScreenDimensions().Width, RunInformation::GetScreenDimensions().Height, 0, 0.1f, 100.0f);

	ID3DX11EffectMatrixVariable* viewMatrixVariable = m_renderingEffect->GetVariable( "View" )->AsMatrix();
	ID3DX11EffectMatrixVariable* projectionMatrixVariable = m_renderingEffect->GetVariable( "Projection" )->AsMatrix();

	// These won't change at all (for now).
	viewMatrixVariable->SetMatrix((float*)&viewMatrix);
	projectionMatrixVariable->SetMatrix((float*)&projectionMatrix);
}


JBRocketRenderInterfaceD3D::~JBRocketRenderInterfaceD3D(void)
{
	// Delete the effect.
	delete m_renderingEffect;
}

// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
Rocket::Core::CompiledGeometryHandle JBRocketRenderInterfaceD3D::CompileGeometry(
	Rocket::Core::Vertex* vertices, int numVertices, 
	int* indices, int numIndices,
	Rocket::Core::TextureHandle texture)
{
	CompiledGeometry* geometry = new CompiledGeometry();

	// Create the vertex buffer first.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof( vertex ) * numVertices; // This is just for quads.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	m_renderer->GetDevice()->CreateBuffer( &bd, NULL, &geometry->m_vertexBuffer );

	// Now the index buffer.
	bd.ByteWidth = sizeof( unsigned int ) * numIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_renderer->GetDevice()->CreateBuffer( &bd, NULL, &geometry->m_indexBuffer );

	// Fill the buffers - vertex first.
	vertex* v = NULL;
	D3D11_MAPPED_SUBRESOURCE mappedBuffer;
	m_renderer->GetDeviceContext()->Map(geometry->m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	v = (vertex*)mappedBuffer.pData;
	for (int i = 0; i < numVertices; ++i)
	{
		v[i] = vertex( 
				XMFLOAT3(vertices[i].position.x,vertices[i].position.y,0),
				XMFLOAT4(
					vertices[i].colour.red / 255.0f,
					vertices[i].colour.green / 255.0f,
					vertices[i].colour.blue / 255.0f,
					vertices[i].colour.alpha / 255.0f),
				XMFLOAT2(vertices[i].tex_coord[0],vertices[i].tex_coord[1]) 
			);
	}
	m_renderer->GetDeviceContext()->Unmap(geometry->m_vertexBuffer, 0);
	// Now Index.
	unsigned int* ind;
	m_renderer->GetDeviceContext()->Map(geometry->m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
	ind = (unsigned int*)mappedBuffer.pData;
	memcpy(ind, indices, sizeof(unsigned int) * numIndices);
	m_renderer->GetDeviceContext()->Unmap(geometry->m_indexBuffer, 0);
	geometry->m_numIndices = (unsigned int)numIndices;
	
	// Set the texture.
	geometry->m_texture = texture == NULL ? nullptr : reinterpret_cast<Texture2DD3D*>(texture);

	// Return this as a geometry handle.
	return reinterpret_cast<Rocket::Core::CompiledGeometryHandle>(geometry);
}

// Called by Rocket when it wants to render application-compiled geometry.
void JBRocketRenderInterfaceD3D::RenderCompiledGeometry(
	Rocket::Core::CompiledGeometryHandle geometryHandle, const Rocket::Core::Vector2f& translation)
{
	// Get the device
	ID3D11DeviceContext* deviceContext = m_renderer->GetDeviceContext();
	// And the geo.
	CompiledGeometry* geometry = reinterpret_cast<CompiledGeometry*>(geometryHandle);		
	
	// Set the buffers for rendering
	UINT stride = sizeof( vertex );
	UINT offset = 0;
	deviceContext->IASetVertexBuffers( 0, 1, &geometry->m_vertexBuffer, &stride, &offset );
	deviceContext->IASetIndexBuffer( geometry->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set primitive topology 
	deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// Set the input layout for the vertices we're about to work with.
	deviceContext->IASetInputLayout(m_renderingEffect->GetInputLayout());

	// Now figure out the world transform.
	XMMATRIX w = XMMatrixTranslation(translation.x, translation.y, 0);
	ID3DX11EffectMatrixVariable* worldMatrixVariable = m_renderingEffect->GetVariable( "World" )->AsMatrix();
	worldMatrixVariable->SetMatrix((float*)&w);

	// Get technique description (required for rendering)
	ID3DX11EffectTechnique* technique = nullptr;

	// Set the texture...
	if (geometry->m_texture)
	{
		ID3DX11EffectShaderResourceVariable* textureVar = m_renderingEffect->GetVariable( "tex2D" )->AsShaderResource();
		textureVar->SetResource( geometry->m_texture->GetShaderResourceViewD3D() );
		technique = m_renderingEffect->GetTechnique(TEXTURED_EFFECT_NAME_UI);
	}
	else 
	{
		technique = m_renderingEffect->GetTechnique(UNTEXTURED_EFFECT_NAME_UI);
	}

	// Get technique description (required for rendering)
	D3DX11_TECHNIQUE_DESC techDesc;
	technique->GetDesc( &techDesc );

	// Draw the technique!
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		// Apply technique pass
		technique->GetPassByIndex( p )->Apply( 0, deviceContext );

		// Draw
		deviceContext->DrawIndexed( geometry->m_numIndices, 0, 0 );
	}
}

// Called by Rocket when it wants to release application-compiled geometry.
void JBRocketRenderInterfaceD3D::ReleaseCompiledGeometry(
	Rocket::Core::CompiledGeometryHandle geometryHandle)
{
	CompiledGeometry* geometry = reinterpret_cast<CompiledGeometry*>(geometryHandle);
	if (geometry != nullptr)
		delete geometry;
}

// Called by Rocket when it wants to enable or disable scissoring to clip content.
void JBRocketRenderInterfaceD3D::EnableScissorRegion(
	bool enable)
{
	m_renderer->EnableScissor(enable);	
}

// Called by Rocket when it wants to change the scissor region.
void JBRocketRenderInterfaceD3D::SetScissorRegion(
	int x, int y, int width, int height)
{
	D3D11_RECT rects[1];
	rects[0].left = x;
	rects[0].right = x + width;
	rects[0].top = y;
	rects[0].bottom = y + height;

	m_renderer->GetDeviceContext()->RSSetScissorRects( 1, rects );
}

// Called by Rocket when a texture is required by the library.
bool JBRocketRenderInterfaceD3D::LoadTexture(
	Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, 
	const Rocket::Core::String& source)
{
	Texture2DD3D* texture = new Texture2DD3D();
	texture->LoadTextureFromFile(source.CString(), *m_renderer);
	textureDimensions.x = texture->GetWidth();
	textureDimensions.y = texture->GetHeight();
	textureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
bool JBRocketRenderInterfaceD3D::GenerateTexture(
	Rocket::Core::TextureHandle& textureHandle, const byte* source, 
	const Rocket::Core::Vector2i& sourceDimensions)
{
	// Create a texture.
	Texture2DD3D* texture = new Texture2DD3D();
	texture->CreateTextureFromArray(
		source,
		Eigen::Vector2i(sourceDimensions.x, sourceDimensions.y),
		*m_renderer);
	// Return the texture.
	textureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(texture);
	return true;
}

// Called by Rocket when a loaded texture is no longer required.
void JBRocketRenderInterfaceD3D::ReleaseTexture(Rocket::Core::TextureHandle texture_handle)
{
	Texture2DD3D* texture = reinterpret_cast<Texture2DD3D*>(texture_handle);
	delete texture;
}

// Returns the native horizontal texel offset for the renderer.
float JBRocketRenderInterfaceD3D::GetHorizontalTexelOffset()
{
	return 0;
}

// Returns the native vertical texel offset for the renderer.
float JBRocketRenderInterfaceD3D::GetVerticalTexelOffset()
{
	return 0;
}

JBRocketRenderInterfaceD3D::CompiledGeometry::CompiledGeometry() :
	m_vertexBuffer(nullptr),
	m_indexBuffer(nullptr),
	m_texture(nullptr),
	m_numIndices(0)
{}

JBRocketRenderInterfaceD3D::CompiledGeometry::~CompiledGeometry()
{
	if (m_vertexBuffer)
		m_vertexBuffer->Release();
	if (m_indexBuffer)
		m_indexBuffer->Release();
}