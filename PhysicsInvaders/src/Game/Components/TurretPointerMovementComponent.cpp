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
#include "TurretPointerMovementComponent.h"

// Project headers
#include "TurretYokeComponent.h"
#include "Input/InputSystem.h"
#include "Utility/Helpers.h"
#include "Game/GameContext.h"
#include "CoreComponents/Box2DBodyComponent.h"
#include "CoreComponents/CameraComponent.h"
#include "ComponentModel/EntityComponentManager.h"

// Standard headers
#include <math.h>

// Box 2D
#include <Box2D/Box2D.h>

// Screen edge const
const float SCREEN_EDGE_MIN = 0.8f;
const float SCREEN_EDGE_MAX = 0.9f;
const float SCREEN_EDGE_SIZE = SCREEN_EDGE_MAX - SCREEN_EDGE_MIN;
const float MAX_DIST_FOR_TWEEN = 32;

TurretPointerMovementComponent::TurretPointerMovementComponent(void) :
	m_yoke(nullptr),
	m_body(nullptr),
	m_movementMethod(TPMM_DESIRED_LOCATION),
	m_cameraLeft(0.0f),
	m_cameraRight(0.0f)
{
}

TurretPointerMovementComponent::~TurretPointerMovementComponent(void)
{
}

void TurretPointerMovementComponent::Initialise(const GameContext& gameContext)
{
	m_yoke = m_entity->GetComponentByType<TurretYokeComponent>();
	m_body = m_entity->GetComponentByTypeFast<Box2DBodyComponent>();
	// Hardcoded name of camera here. Ideally would get component by type, but for
	// now just trying to fix coordinate bug
	ComponentModel::Entity* camera = gameContext.GetComponentManager().FindEntityByName("Camera");
	CameraComponent* cameraComponent = camera->GetComponentByTypeFast<CameraComponent>();
	m_cameraLeft = cameraComponent->GetOrthographicRect().m_left;
	m_cameraRight = cameraComponent->GetOrthographicRect().m_right;
}

void TurretPointerMovementComponent::Cleanup(const GameContext& /*gameContext*/)
{
	m_body = nullptr;
	m_yoke = nullptr;
}

void TurretPointerMovementComponent::PhysicsUpdate(const GameTime& /*time*/, const GameContext& /*gameContext*/)
{
	float targetX = 0.0f;
	if (InputSystem::IsActive() && InputSystem::GetCurrentPointerState().GetPositionValid())
	{
		switch (m_movementMethod)
		{
		case TPMM_SCREEN_EDGE:
			targetX = GetDesiredXScreenEdge((InputSystem::GetCurrentPointerState().GetXNormalised() * 2) - 1);
			break;
		case TPMM_DESIRED_LOCATION:
			targetX = GetDesiredXDesiredLoc((float)InputSystem::GetCurrentPointerState().GetX(), (float)InputSystem::GetCurrentPointerState().GetXMax());
			break;
		}

		// Sort out whether to fire.
		if (InputSystem::GetCurrentPointerState().GetLeftDown())
		{
			m_yoke->Fire();
		}
	}
	m_yoke->SetDesiredDirection(targetX);
}

float TurretPointerMovementComponent::GetDesiredXScreenEdge(float currentPointerPosX) const
{
	// Sort out which direction to move in
	int direction = Helpers::Sign(currentPointerPosX);
	float power = (Helpers::Clamp(abs(currentPointerPosX), SCREEN_EDGE_MIN, SCREEN_EDGE_MAX) - SCREEN_EDGE_MIN) / SCREEN_EDGE_SIZE;
	return power * direction;
}

float TurretPointerMovementComponent::GetDesiredXDesiredLoc(float currentPointerPosX, float maxX) const
{
	float xRange = m_cameraRight - m_cameraLeft;
	float xPos = (-m_body->GetBody()->GetPosition().x * BOX2D_SCALE_FACTOR) - m_cameraLeft;
	float bodyPos = (xPos / xRange) * maxX;
	float difference = (currentPointerPosX - bodyPos) / MAX_DIST_FOR_TWEEN;
	float delta = Helpers::Clamp(difference, -1.f, 1.f);
	if (abs(delta) < 0.01f)
	{
		delta = 0;
	}
	else if (Helpers::Sign(delta) != Helpers::Sign(-m_body->GetBody()->GetLinearVelocity().x))
	{
		delta = Helpers::Sign(delta) * 1.f;
	}
	return delta;
}