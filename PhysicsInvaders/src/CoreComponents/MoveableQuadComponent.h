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

// Forward declarations
class MoveableTexturedQuad;
class QuadRendererD3D;

// Base header
#include "ComponentModel/Component.h"

// Eigen types.
#include <Core/EigenIncludes.h>

class MoveableQuadComponent : public ComponentModel::Component
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

public:
	MoveableQuadComponent(void);
	~MoveableQuadComponent(void);

	/// Sets the moveable quad this should use. The component will destroy the quad when it cleans
	/// itself up.
	void SetQuad(MoveableTexturedQuad* quad) { m_quad = quad; }

	/// Returns a pointer to the body.
	MoveableTexturedQuad* GetQuad() const { return m_quad; }

	/// Sets the renderer to use - really this should not be platform specific.
	void SetRenderer(QuadRendererD3D* renderer) { m_renderer = renderer; }
		
	/// Prop transform changes from the entity to the quad.
	virtual void SynchroniseRenderData(const GameContext& /*gameContext*/);

	/// Add the quad to the quad renderer's render queue.
	virtual void RenderUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Delete the quad
	virtual void Cleanup(const GameContext& gameContext);

	/// Set colour
	inline void SetColour(const Eigen::Vector4f& colour) { m_colour = colour; }

public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return true; }
	static bool HasSynchroniseRenderData() { return true; }

private:
	MoveableTexturedQuad* m_quad;
	QuadRendererD3D* m_renderer;
	Eigen::Vector4f m_colour;
};

