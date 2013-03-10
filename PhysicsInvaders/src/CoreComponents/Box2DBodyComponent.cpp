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
#include "Box2DBodyComponent.h"

// Box 2D
#include <Box2D/Box2D.h>

// Project headers
#include "Physics/EigenToBox2D.h"
#include "Core/GameTime.h"
#include "Core/ITimeSource.h"
#include "Utility/Helpers.h"

Box2DBodyComponent::Box2DBodyComponent(void) :
	m_body(nullptr),
	m_mostRecentPos(b2Vec2_zero),
	m_previousPos(b2Vec2_zero)
{
}

Box2DBodyComponent::~Box2DBodyComponent(void)
{
}

void Box2DBodyComponent::ApplyForceToCenter(const b2Vec2& force)
{
	m_body->ApplyForceToCenter(force);
}

void Box2DBodyComponent::FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	m_mostRecentPos = m_body->GetPosition();
	m_previousPos = m_body->GetPosition();
	m_mostRecentRot = m_body->GetAngle();
	m_previousRot = m_mostRecentRot;
	m_previousPhysicsTime = time.GetPhysicsTime()->GetCurrentTime();
}

void Box2DBodyComponent::CoreUpdate(const GameTime& time, const GameContext& /*gameContext*/)
{
	AppTicks physicsTime = time.GetPhysicsTime()->GetCurrentTime();
	if (physicsTime != m_previousPhysicsTime)
	{
		m_previousPhysicsTime = physicsTime;
		m_previousPos = m_mostRecentPos;
		m_mostRecentPos = m_body->GetPosition();
		m_previousRot = m_mostRecentRot;
		m_mostRecentRot = m_body->GetAngle();
	}
	float t = (time.GetGameTime()->GetCurrentTime() - time.GetPhysicsTime()->GetCurrentTime()) / (float)time.GetStep();
	m_entity->SetPosition(EigenToBox2D::Box2DVector2ToEigenVector3(BOX2D_SCALE_FACTOR * Helpers::Lerp<b2Vec2>(m_previousPos, m_mostRecentPos, t), m_entity->GetPosition()[2]));
	m_entity->SetOrientation(Eigen::Quaternionf(Eigen::AngleAxisf(Helpers::Lerp<float>(m_previousRot, m_mostRecentRot, t), Eigen::Vector3f(0, 0, 1))));
}

void Box2DBodyComponent::Cleanup(const GameContext& /*gameContext*/)
{
	if (m_body != nullptr)
	{
		m_body->GetWorld()->DestroyBody(m_body);
		m_body = nullptr;
	}
}

void Box2DBodyComponent::SetBody(b2Body* body)
{ 
	m_body = body;
	// Allow the body to get back to its entity.
	m_body->SetUserData(this->m_entity);
}