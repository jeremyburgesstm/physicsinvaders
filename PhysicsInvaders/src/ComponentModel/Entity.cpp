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

#include <algorithm>

#include "Entity.h"
#include "Component.h"
#include "EntityComponentManager.h"

ComponentModel::Entity::Entity() :
	m_position(Eigen::Vector3f::Zero()),
	m_orientation(Eigen::Quaternionf::Identity()),
	m_scale(1),
	m_enabled(true),
	m_isAlive(false)
{}

void ComponentModel::Entity::AddComponent(Component* c)
{
	m_components.push_back(c);
	c->SetEntity(this);
}

void ComponentModel::Entity::RemoveComponent(Component* c)
{
	c->SetEntity(nullptr);
	m_components.erase(std::find(m_components.begin(), m_components.end(), c));
}

void ComponentModel::Entity::RemoveAllComponents(ComponentModel::EntityComponentManager* ecm)
{
	while (!m_components.empty())
	{
		ecm->RemoveComponent(this, m_components.back());
	}
}

Eigen::Affine3f ComponentModel::Entity::GetTransform() const
{
	Eigen::Affine3f transform(Eigen::Affine3f::Identity());
	transform.prerotate(m_orientation);
	transform.prescale(m_scale);
	transform.pretranslate(m_position);
	return transform;
}

void ComponentModel::Entity::Destroy()
{
	m_manager->ReleaseEntity(this);
}