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

// Need this for Windows typedefs unfortunately.
#include <Windows.h>
#include <vector>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11DeviceContext;

/**
 * \class RendererD3D
 *
 * Our Direct 3D Renderer. Creating it will initialise the context, and get ready for rendering.
 *
 * @author Jeremy Burgess
 */
class RendererD3D
{
public:
	RendererD3D(HWND hWnd);
	~RendererD3D(void);

	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void FlipBuffers();
	void EnableScissor(bool enable);

	ID3D11Device* GetDevice() const
	{
		return m_pd3dDevice;
	}

	ID3D11DeviceContext* GetDeviceContext() const
	{
		return m_pd3dDeviceContext;
	}
private:

	ID3D11Device* m_pd3dDevice;
	ID3D11DeviceContext* m_pd3dDeviceContext;
	IDXGISwapChain* m_swapChain;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11RasterizerState* m_rasterizerStateNoScissor;
	ID3D11RasterizerState* m_rasterizerStateScissor;
	ID3D11RasterizerState* m_rasterizerStateCurrent;
};

