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

// Included for internally used types.
#include <vector>

// Forward declarations
class RendererD3D;
class EffectD3D;
class MoveableTexturedQuad;
struct ID3D11Buffer;
class ICamera;

/**
 * \class ITexture2D
 *
 * Simple quad renderer that adds quads to an internal list each frame, draws them
 * using a owned effect that it manages, and then clears its internal list.
 */
class QuadRendererD3D
{
public:
	QuadRendererD3D(
		const RendererD3D& renderer, 
		const char* effectFilename, 
		size_t numReservedQuadSpots);
	~QuadRendererD3D(void);

	void AddToRenderList(const MoveableTexturedQuad* quad);
	void Render(const RendererD3D& renderer, const ICamera* camera);
	void ClearRenderList();

private:
	std::vector<const MoveableTexturedQuad*> m_quadsForRendering;
	EffectD3D* m_renderingEffect;
	// Own a vert buffer for drawing the quads
	ID3D11Buffer* m_buffer;
};

