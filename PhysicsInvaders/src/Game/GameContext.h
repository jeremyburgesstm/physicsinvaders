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

class GameMessageHub;
class b2World;
class RendererD3D;
class QuadRendererD3D;
class TextureManager;
namespace ComponentModel
{
	class EntityComponentManager;
}

// Not a copyable class
#include <boost/noncopyable.hpp>

/**
 * This class acts as a 'hub' class for entities necessary throughout 
 * code, which other entities may need access to - for instance the 
 * messaging system. We use this instead of singletons in the 
 * game code, however because I don't want to pass this to every single
 * update, entities are expected to cache whatever they need.
 * 
 * The objects in here are persistent beyond the scope of any in-game
 * entities, and can be assumed to outlive anything other than the 
 * game world.
 */
class GameContext : public boost::noncopyable
{
public: 
	GameContext(GameMessageHub& messageHub, 
		b2World& box2DWorld, 
		ComponentModel::EntityComponentManager& componentManager,
		const RendererD3D& renderer,
		QuadRendererD3D& quadRenderer,
		TextureManager& textureManager) :
		m_messageHub(messageHub),
		m_box2dWorld(box2DWorld),
		m_componentManager(componentManager),
		m_renderer(renderer),
		m_quadRenderer(quadRenderer),
		m_textureManager(textureManager)
	{}

	/// Returns a reference to the message hub (for events/communications).
	GameMessageHub& GetMessageHub() const { return m_messageHub; }

	/// Returns a reference to the component manager.
	ComponentModel::EntityComponentManager& GetComponentManager() const { return m_componentManager; }

	/// Returns a reference to the b2d world.
	b2World& GetBox2DWorld() const { return m_box2dWorld; }
	
	/// Returns a reference to the component manager.
	const RendererD3D& GetRenderer() const { return m_renderer; }

	/// Returns a reference to the b2d world.
	QuadRendererD3D& GetQuadRenderer() const { return m_quadRenderer; }

	/// Returns a reference to the texture manager
	TextureManager& GetTextureManager() const { return m_textureManager; }

private:
	GameMessageHub& m_messageHub;
	b2World& m_box2dWorld;
	ComponentModel::EntityComponentManager& m_componentManager;
	const RendererD3D& m_renderer;
	QuadRendererD3D& m_quadRenderer;
	TextureManager& m_textureManager;
};