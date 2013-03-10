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

#pragma once

#include <string>
#include <Core/EigenIncludes.h>

// Base Class
#include "ITexture2D.h"

// Forward Declarations
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct D3D11_TEXTURE2D_DESC;
class RendererD3D;

/**
 * \class Texture2DD3D
 *
 * Texture wrapper class for D3D. 
 */
class Texture2DD3D : public ITexture2D
{
public:
	/// Default C-Tor - initialises internal vars to empty, state to unloaded.
	Texture2DD3D();
	/// Destructor - releases all resources.
	~Texture2DD3D();

	/// Loads this texture from a file
	void LoadTextureFromFile(const std::string& filename, const RendererD3D& renderer)
	{
		LoadTextureFromFile(filename.c_str(), renderer);
	}
	void LoadTextureFromFile(const char* filename, const RendererD3D& renderer);

	/// Creates this texture from a char array.
	void CreateTextureFromArray(const unsigned char* sourceData, const Eigen::Vector2i& sourceDimensions, const RendererD3D& renderer);

	/// \name Accessors / Mutators
	/// @{

		/// Returns if the texture is loaded
		inline bool GetLoaded() const
		{
			return m_textureDescription != nullptr;
		}

		/// Returns the texture's width
		virtual unsigned int GetWidth() const;
		/// Returns the texture's height
		virtual unsigned int GetHeight() const;

		/// Returns the texture's resource view (required for use in effects).
		inline ID3D11ShaderResourceView* GetShaderResourceViewD3D() const
		{
			return m_shaderResourceView;
		}

	/// @}

private:
	/// Texture pointer itself. May not be required.
	ID3D11Texture2D* m_texture;
	/// Shader resource view. Required for use in a shader.
	ID3D11ShaderResourceView* m_shaderResourceView;
	/// Description of the texture.
	D3D11_TEXTURE2D_DESC* m_textureDescription;
};

