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

#include "CoreComponents\CameraComponent.h"

CameraComponent::CameraComponent(void) :
	m_transform(Eigen::Affine3f::Identity()),
	m_cameraType(CAMERA_TYPE_PERSPECTIVE),
	m_farClip(100.f),
	m_nearClip(0.01f),
	m_orthoRect(0.f, 0.f, 0.f, 0.f),
	m_fovAngleY((float)M_PI / 2.f),
	m_aspectMultiplier(1.f)
{
}

CameraComponent::~CameraComponent(void)
{
}

void CameraComponent::SynchroniseRenderData(const GameContext& /*gameContext*/)
{
	m_transform = m_entity->GetTransform();
	m_renderCameraType = m_cameraType;
	m_renderFarClip = m_farClip;
	m_renderNearClip = m_nearClip;
	m_renderOrthoRect = m_orthoRect;
	m_renderFovAngleY = m_fovAngleY;
	m_renderAspectMultiplier = m_aspectMultiplier;
	m_renderPlane = Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, m_renderNearClip));
}

// Returns the position of the camera
Eigen::Vector3f CameraComponent::GetRenderPosition() const
{
	return m_transform.translation();
}

// Returns the focus position of the camera
Eigen::Vector3f CameraComponent::GetRenderFocusPosition() const
{
	return m_transform.rotation() * Eigen::Vector3f::UnitZ() + GetRenderPosition();
}

// Returns the up vector of the camera
Eigen::Vector3f CameraComponent::GetRenderUp() const
{
	return m_transform.rotation() * Eigen::Vector3f::UnitY();
}

float CameraComponent::GetRenderDistanceToPoint(Eigen::Vector3f pos) const
{
	switch (m_renderCameraType)
	{
	case ICamera::CAMERA_TYPE_ORTHOGRAPHIC:
		return m_renderPlane.absDistance(pos);
		break;
	case ICamera::CAMERA_TYPE_PERSPECTIVE:
		Eigen::Vector3f dist = GetRenderPosition() - pos;
		return (dist[0] * dist[0] + dist[1] * dist[1] + dist[2] * dist[2]);
		break;
	}
	return FLT_MAX;
}