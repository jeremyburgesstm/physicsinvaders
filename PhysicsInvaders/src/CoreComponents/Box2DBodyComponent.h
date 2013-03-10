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
class b2Body;

// Base header
#include "ComponentModel/Component.h"

// Box2D types
#include <Box2D/Common/b2Math.h>

class Box2DBodyComponent : public ComponentModel::Component
{
public:
	Box2DBodyComponent(void);
	~Box2DBodyComponent(void);

	/// Sets the body this component uses. We'll probably change how this works, once
	/// we figure out how we want to pass around game context stuff.
	void SetBody(b2Body* body);

	/// Returns a pointer to the body.
	b2Body* GetBody() const { return m_body; }

	/// Helper passthrough for b2D method.
	void ApplyForceToCenter(const b2Vec2& force);

	/// Gets the initial positions.
	virtual void FirstCoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);

	/// Core update propagates changes on the body to the entity.
	virtual void CoreUpdate(const GameTime& time, const GameContext& /*gameContext*/);
	
	/// Cleans up the component
	virtual void Cleanup(const GameContext& gameContext);
public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return true; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return false; }


private:
	b2Body* m_body;

	b2Vec2 m_mostRecentPos;
	b2Vec2 m_previousPos;
	float m_mostRecentRot;
	float m_previousRot;
	AppTicks m_previousPhysicsTime;
};

