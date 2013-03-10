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
#include "RendererD3D.h"

// App headers
#include "Core/RunInformation.h"

// D3D headers
#include <D3D11.h>
#include <xnamath.h>

struct vertex
{
	vertex(XMFLOAT3 nPos, XMFLOAT4 nCol, XMFLOAT2 tCoord)
	{
		pos = nPos;
		color = nCol;
		texCoord = tCoord;
	}

	vertex() {}

	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT2 texCoord;
};

RendererD3D::RendererD3D(HWND hWnd)
{
	//Set up DX swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	//set buffer dimensions and format
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = RunInformation::GetScreenDimensions().Width;
	swapChainDesc.BufferDesc.Height = RunInformation::GetScreenDimensions().Height;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//set refresh rate
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	//sampling settings
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1;

	//output window handle
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.Windowed = true;

	//Create the D3D device
	D3D_FEATURE_LEVEL featureLevel;
	/*HRESULT d3dErr = */D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_pd3dDevice,
		&featureLevel,
		&m_pd3dDeviceContext);

	//create viewport structure
	D3D11_VIEWPORT viewPort;
	viewPort.Width = RunInformation::GetScreenDimensions().Width;
	viewPort.Height = RunInformation::GetScreenDimensions().Height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	//set the viewport
	m_pd3dDeviceContext->RSSetViewports(1, &viewPort);

	// Setup the rasterizer
	D3D11_RASTERIZER_DESC rasterizerState;
	rasterizerState.CullMode = D3D11_CULL_NONE;
	rasterizerState.FillMode = D3D11_FILL_SOLID;
	rasterizerState.FrontCounterClockwise = true;
	rasterizerState.DepthBias = false;
	rasterizerState.DepthBiasClamp = 0;
	rasterizerState.SlopeScaledDepthBias = 0;
	rasterizerState.DepthClipEnable = true;
	rasterizerState.ScissorEnable = false;
	rasterizerState.MultisampleEnable = false;
	rasterizerState.AntialiasedLineEnable = true;

	m_pd3dDevice->CreateRasterizerState( &rasterizerState, &m_rasterizerStateNoScissor);
	m_rasterizerStateCurrent = m_rasterizerStateNoScissor;
	m_pd3dDeviceContext->RSSetState(m_rasterizerStateCurrent);

	// Create the scissored one for later use.
	rasterizerState.ScissorEnable = true;
	m_pd3dDevice->CreateRasterizerState( &rasterizerState, &m_rasterizerStateScissor);


	//Create render target view

	//try to get the back buffer
	ID3D11Texture2D* pBackBuffer;

	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	//try to create render target view
	m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_renderTargetView);

	//release the back buffer
	pBackBuffer->Release();

	//set the render target
	m_pd3dDeviceContext->OMSetRenderTargets(1, &m_renderTargetView, NULL);
}

RendererD3D::~RendererD3D(void)
{
	if ( m_renderTargetView != nullptr )
		m_renderTargetView->Release();

	if ( m_swapChain != nullptr )
		m_swapChain->Release();

    if ( m_pd3dDevice != nullptr ) 
        m_pd3dDevice->Release();

	if ( m_rasterizerStateScissor != nullptr )
		m_rasterizerStateScissor->Release();

	if ( m_rasterizerStateNoScissor != nullptr )
		m_rasterizerStateNoScissor->Release();

}

void RendererD3D::Clear(float r, float g, float b, float a)
{
	float colours[4] = {r, g, b, a};
	
	m_pd3dDeviceContext->ClearRenderTargetView(m_renderTargetView, colours);
}

void RendererD3D::FlipBuffers()
{
	m_swapChain->Present(0, 0);
}

void RendererD3D::EnableScissor(bool enable)
{
	if (enable && m_rasterizerStateCurrent != m_rasterizerStateScissor)
	{
		m_rasterizerStateCurrent = m_rasterizerStateScissor;
		m_pd3dDeviceContext->RSSetState(m_rasterizerStateCurrent);
	}
	else if (!enable && m_rasterizerStateCurrent != m_rasterizerStateNoScissor)
	{
		m_rasterizerStateCurrent = m_rasterizerStateNoScissor;
		m_pd3dDeviceContext->RSSetState(m_rasterizerStateCurrent);
	}
}