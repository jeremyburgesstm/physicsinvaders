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
#include "EffectD3D.h"

// D3D 10 headers
#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX11effect.h>

// Project headers
#include "RendererD3D.h"

EffectD3D::EffectD3D() :
	m_effect(nullptr),
	m_effectIALayout(nullptr)
{
}

EffectD3D::~EffectD3D(void)
{
	// Only need to release the Effect.
	if (m_effect)
		m_effect->Release();

	if (m_effectIALayout)
		m_effectIALayout->Release();
}

// Loads this texture from a file
void EffectD3D::LoadEffectFromFile(const char* filename, const RendererD3D& renderer)
{
	// First check if we're already loaded:
	if (m_effect != nullptr)
	{
		m_effect->Release();
		m_effect = nullptr;
	}

	// And the layout...
	if (m_effectIALayout != nullptr)
	{
		m_effectIALayout->Release();
		m_effectIALayout = nullptr;
	}

	// For now just load synchronously.
	ID3D10Blob* effectBlob;
	ID3D10Blob* errorBlob;
	HRESULT result;
	D3DX11CompileFromFile(	filename,
								NULL,
								NULL,
								NULL,
								"fx_5_0",
								0, 
								0,
								NULL,
								&effectBlob,
								&errorBlob,
								&result );
	
	/*HRESULT err = */D3DX11CreateEffectFromMemory(
		effectBlob->GetBufferPointer(),
		effectBlob->GetBufferSize(),
		0,
		renderer.GetDevice(),
		&m_effect);
}

// Returns a technique if it can.
ID3DX11EffectTechnique* EffectD3D::GetTechnique(const std::string& name) const
{
	return m_effect ? m_effect->GetTechniqueByName(name.c_str()) : nullptr;
}

// Returns a variable if it can.
ID3DX11EffectVariable* EffectD3D::GetVariable(const std::string& name) const
{
	return m_effect ? m_effect->GetVariableByName(name.c_str()) : nullptr;
}

// Create an input assembler layout for the vert data for this shader.
// Note that we could share this across multiple shaders - worth considering
// having a layout cache and a ref counting system.
void EffectD3D::CreateInputLayout(const std::string& techniqueName, 
	const D3D11_INPUT_ELEMENT_DESC* inputDescription, 
	unsigned int numInputElements,
	const RendererD3D& renderer)
{
	// Get the technique
	ID3DX11EffectTechnique* technique = GetTechnique(techniqueName.c_str());
	
	// If the technique is invalid, don't continue.
	if (!m_effect || !technique)
	{
		return;
	}

	// Get the descriptor of pass 0 (we assume it's the same for all passes).
	D3DX11_PASS_DESC passDesc;
	technique->GetPassByIndex( 0 )->GetDesc( &passDesc );
	
	// Create the input layout
	renderer.GetDevice()->CreateInputLayout( inputDescription,
									 numInputElements,
									 passDesc.pIAInputSignature,
									 passDesc.IAInputSignatureSize,
									 &m_effectIALayout );
}