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

class RendererD3D;
class EffectD3D;
class Texture2DD3D;
struct ID3D11Buffer;
// To avoid including crazy windows headers, we typedef this here.
typedef unsigned char byte;

#include <Rocket\Core\RenderInterface.h>

/**
 * Our version of the Rocket Render Interface, customised for D3D11.
 *
 * Duplicates a lot of code from the QuadRenderer.
 * TODO: Consider creating common backend for QuadRenderer & this.
 */
class JBRocketRenderInterfaceD3D : public Rocket::Core::RenderInterface
{
public:
	JBRocketRenderInterfaceD3D(
		RendererD3D* renderer,
		const char* effectFilename);
	virtual ~JBRocketRenderInterfaceD3D(void);
	
	/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	/// Not supported in our D3D renderer - it's highly inefficient to render anything without
	/// pre-built buffers in D3D10.
	virtual void RenderGeometry(
		Rocket::Core::Vertex* /*vertices*/, int /*num_vertices*/, 
		int* /*indices*/, int /*num_indices*/, 
		Rocket::Core::TextureHandle /*texture*/, const Rocket::Core::Vector2f& /*translation*/)
	{}

	/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(
		Rocket::Core::Vertex* vertices, int numVertices, 
		int* indices, int numIndices,
		Rocket::Core::TextureHandle texture);

	/// Called by Rocket when it wants to render application-compiled geometry.
	virtual void RenderCompiledGeometry(
		Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);
	/// Called by Rocket when it wants to release application-compiled geometry.
	virtual void ReleaseCompiledGeometry(
		Rocket::Core::CompiledGeometryHandle geometry);

	/// Called by Rocket when it wants to enable or disable scissoring to clip content.
	virtual void EnableScissorRegion(
		bool enable);
	/// Called by Rocket when it wants to change the scissor region.
	virtual void SetScissorRegion(
		int x, int y, int width, int height);

	/// Called by Rocket when a texture is required by the library.
	virtual bool LoadTexture(
		Rocket::Core::TextureHandle& textureHandle, Rocket::Core::Vector2i& textureDimensions, 
		const Rocket::Core::String& source);
	/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	virtual bool GenerateTexture(
		Rocket::Core::TextureHandle& textureHandle, const byte* source, 
		const Rocket::Core::Vector2i& sourceDimensions);
	/// Called by Rocket when a loaded texture is no longer required.
	virtual void ReleaseTexture(Rocket::Core::TextureHandle textureHandle);

	/// Returns the native horizontal texel offset for the renderer.
	float GetHorizontalTexelOffset();
	/// Returns the native vertical texel offset for the renderer.
	float GetVerticalTexelOffset();

private:
	struct CompiledGeometry
	{
		CompiledGeometry();
		~CompiledGeometry();

		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		unsigned int m_numIndices;
		Texture2DD3D* m_texture;
	};

private:
	/// Pointer to our D3D Renderer
	RendererD3D* m_renderer;

	/// The effect we use to render this.
	EffectD3D* m_renderingEffect;
};

