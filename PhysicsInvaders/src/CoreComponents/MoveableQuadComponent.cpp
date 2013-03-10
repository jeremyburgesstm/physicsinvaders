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
#include "MoveableQuadComponent.h"

// Project headers
#include "Graphics/MoveableTexturedQuad.h"
#include "Graphics/QuadRendererD3D.h"

MoveableQuadComponent::MoveableQuadComponent(void) :
	m_colour(Eigen::Vector4f::Ones())
{
}

MoveableQuadComponent::~MoveableQuadComponent(void)
{
}

void MoveableQuadComponent::SynchroniseRenderData(const GameContext& /*gameContext*/)
{
	m_quad->SetPosition(m_entity->GetPosition());
	m_quad->SetOrientation(m_entity->GetOrientation());	
	m_quad->SetColour(m_colour);
}

void MoveableQuadComponent::RenderUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/)
{
	m_renderer->AddToRenderList(m_quad);
}

void MoveableQuadComponent::Cleanup(const GameContext& /*gameContext*/)
{
	delete m_quad;
	m_quad = nullptr;
	m_renderer = nullptr;
	m_colour = Eigen::Vector4f::Ones();
}