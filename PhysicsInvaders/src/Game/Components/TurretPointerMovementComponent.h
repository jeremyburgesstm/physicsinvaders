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
class TurretYokeComponent;
class Box2DBodyComponent;

// Base header
#include "ComponentModel/Component.h"

class TurretPointerMovementComponent : public ComponentModel::Component
{
public:
	enum TurretPointerMovementMethod
	{
		TPMM_SCREEN_EDGE,
		TPMM_DESIRED_LOCATION
	};

public:
	TurretPointerMovementComponent(void);
	~TurretPointerMovementComponent(void);
	
	/// Initialises the component
	virtual void Initialise(const GameContext& gameContext);
	
	/// Nullifies all internal refs
	virtual void Cleanup(const GameContext& gameContext);

	/// Listens for input, and does what needs to be done.
	virtual void PhysicsUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Sets the input method
	void SetInputMethod(TurretPointerMovementMethod method) { m_movementMethod = method; }

public:
	static bool HasPhysicsUpdate() { return true; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }

private:
	float GetDesiredXScreenEdge(float currentPointerPosX) const;
	float GetDesiredXDesiredLoc(float currentPointerPosX, float maxX) const;

private:
	TurretYokeComponent* m_yoke;
	Box2DBodyComponent* m_body;
	TurretPointerMovementMethod m_movementMethod;
	float m_cameraLeft;
	float m_cameraRight;
};

