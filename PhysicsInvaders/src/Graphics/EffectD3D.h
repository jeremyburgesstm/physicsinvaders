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
#include <hash_map>

struct ID3DX11Effect;
struct ID3DX11EffectTechnique;
struct ID3DX11EffectVariable;
struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;
class RendererD3D;

/**
 * \class EffectD3D
 *
 * Effect wrapper class for D3D. Not sure if it makes sense to abstract this, as it 
 * can't be used without understanding D3D semantics.
 */
class EffectD3D
{
public:
	EffectD3D();
	~EffectD3D();

		/// Loads this texture from a file
	void LoadEffectFromFile(const std::string& filename, const RendererD3D& renderer)
	{
		LoadEffectFromFile(filename.c_str(), renderer);
	}
	void LoadEffectFromFile(const char* filename, const RendererD3D& renderer);

	/// \name Accessors / Mutators
	/// @{

		/// Returns if the texture is loaded
		inline bool GetLoaded() const
		{
			return m_effect != nullptr;
		}

		/// Returns the effect.
		inline ID3DX11Effect* GetEffect() const
		{
			return m_effect;
		}

		/// Returns a technique if it can.
		ID3DX11EffectTechnique* GetTechnique(const std::string& name) const;

		/// Returns a variable if it can.
		ID3DX11EffectVariable* GetVariable(const std::string& name) const;

		/// Gets the layout that has been bound for this effect.
		inline ID3D11InputLayout* GetInputLayout() const
		{
			return m_effectIALayout;
		}

		/// Creates the layout to be used.
		void CreateInputLayout(const std::string& techniqueName, 
			const D3D11_INPUT_ELEMENT_DESC* inputDescription, 
			unsigned int numInputElements,
			const RendererD3D& renderer);

	/// @}

private:
	ID3DX11Effect* m_effect;
	ID3D11InputLayout* m_effectIALayout;
};