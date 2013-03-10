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

#include "TextureManager.h"

#include "ITexture2D.h"

TextureManager::~TextureManager()
{
	for (auto texPairIt = m_textures.begin(); texPairIt != m_textures.end(); ++texPairIt)
	{
		delete (*texPairIt).second;
	}
	m_textures.clear();
}

// Adds a texture to the dictionary.
void TextureManager::AddTexture(const std::string& name, ITexture2D* tex)
{
	m_textures[name] = tex;
}

// Destroys the specified texture.
void TextureManager::DestroyTexture(const std::string& name)
{
	if (m_textures.count(name) > 0)
	{
		delete m_textures[name];
		m_textures.erase(name);
	}
}

// Retreives the texture by name, or null if no such texture exists.
const ITexture2D* TextureManager::GetTexture(const std::string& name) const
{
	auto tIt = m_textures.find(name);
	return tIt == m_textures.end() ? nullptr : (*tIt).second;
}
