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
#include "Texture2DD3D.h"

// D3D 10 headers
#include <D3D11.h>
#include <D3DX11.h>

// Project headers
#include "RendererD3D.h"

// Boost headers.
#include <boost/assert.hpp>

Texture2DD3D::Texture2DD3D(void) :
	m_texture(nullptr),
	m_shaderResourceView(nullptr),
	m_textureDescription(nullptr)
{
}

Texture2DD3D::~Texture2DD3D(void)
{
	// Only need to release the shader resource view.
	if (m_shaderResourceView)
		m_shaderResourceView->Release();

	// And delete the texture description (delete is safe on null)
	delete m_textureDescription;
}

// Loads this texture from a file
void Texture2DD3D::LoadTextureFromFile(const char* filename, const RendererD3D& renderer)
{
	// First check if we're already loaded:
	if (m_shaderResourceView != nullptr)
	{
		m_shaderResourceView->Release();
		m_shaderResourceView = nullptr;

		delete m_textureDescription;
	}

	// For now just load synchronously.
	D3DX11CreateShaderResourceViewFromFile( renderer.GetDevice(), 
		filename, NULL, NULL, &m_shaderResourceView, NULL );
	ID3D11Resource* textureResource = NULL;
	D3D11_RESOURCE_DIMENSION type;
	m_shaderResourceView->GetResource(&textureResource);
	textureResource->GetType(&type);
	if (type == D3D11_RESOURCE_DIMENSION_TEXTURE2D)
	{
		m_texture = (ID3D11Texture2D*)textureResource;
		m_textureDescription = new D3D11_TEXTURE2D_DESC;
		m_texture->GetDesc(m_textureDescription);
	}
}

void Texture2DD3D::CreateTextureFromArray(
	const unsigned char* sourceData, 
	const Eigen::Vector2i& sourceDimensions,
	const RendererD3D& renderer	)
{
	// Create the texture pointer itself.
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = sourceDimensions(0);
	desc.Height = sourceDimensions(1);
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	HRESULT created = renderer.GetDevice()->CreateTexture2D( &desc, NULL, &m_texture );
	BOOST_ASSERT(created == 0);

	// Write the data into it.
	D3D11_MAPPED_SUBRESOURCE mappedTex;
	renderer.GetDeviceContext()->Map(
		m_texture, 
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_WRITE_DISCARD,
		0, 
		&mappedTex);

	UCHAR* pTexels = (UCHAR*)mappedTex.pData;
	for( UINT row = 0; row < desc.Height; row++ )
	{
		UINT rowStart = row * mappedTex.RowPitch;
		for( UINT col = 0; col < desc.Width; col++ )
		{
			const byte* source_pixel = sourceData + (sourceDimensions(0) * 4 * row) + (col * 4);

			UINT colStart = col * 4;
			pTexels[rowStart + colStart + 0] = source_pixel[2]; // Red
			pTexels[rowStart + colStart + 1] = source_pixel[1]; // Green
			pTexels[rowStart + colStart + 2] = source_pixel[0];  // Blue
			pTexels[rowStart + colStart + 3] = source_pixel[3];  // Alpha
		}
	}

	renderer.GetDeviceContext()->Unmap( m_texture, D3D11CalcSubresource(0, 0, 1) );

	// Get the description.
	m_textureDescription = new D3D11_TEXTURE2D_DESC;
	m_texture->GetDesc(m_textureDescription);

	// Create the shader resource view.
	renderer.GetDevice()->CreateShaderResourceView(
		m_texture, NULL, &m_shaderResourceView);

	// Done!
}

// Get the width
unsigned int Texture2DD3D::GetWidth() const
{
	if (m_textureDescription)
		return m_textureDescription->Width;
	return 0;
}

// Get the height
unsigned int Texture2DD3D::GetHeight() const
{
	if (m_textureDescription)
		return m_textureDescription->Height;
	return 0;
}