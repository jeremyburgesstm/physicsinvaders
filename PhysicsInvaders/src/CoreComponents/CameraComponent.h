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

// Base header
#include "ComponentModel/Component.h"
#include "Graphics/ICamera.h"

// Eigen
#include "Core/EigenIncludes.h"

class CameraComponent :	public ComponentModel::Component, public ICamera
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

public:
	CameraComponent(void);
	virtual ~CameraComponent(void);

	/// \name Component members
	/// @{
	virtual void SynchroniseRenderData(const GameContext& /*gameContext*/);
	/// @}

	/// \name ICamera Members + relevant setters
	/// @{
		/// Gets the type of camera.
		virtual CameraType GetCameraType() const { return m_cameraType; }
		virtual CameraType GetRenderCameraType() const { return m_renderCameraType; }
		void SetCameraType(CameraType cameraType) { m_cameraType = cameraType; }

		/// Returns the position of the camera
		virtual Eigen::Vector3f GetRenderPosition() const;
		/// Returns the focus position of the camera
		virtual Eigen::Vector3f GetRenderFocusPosition() const;
		/// Returns the up vector of the camera
		virtual Eigen::Vector3f GetRenderUp() const;

		/// Gets the far clip distance of the camera
		virtual float GetFarClipDistance() const { return m_farClip; }
		virtual float GetRenderFarClipDistance() const { return m_renderFarClip; }
		void SetFarClipDistance(float farClip) { m_farClip = farClip; }

		/// Gets the near clip distance of the camera
		virtual float GetNearClipDistance() const { return m_nearClip; }
		virtual float GetRenderNearClipDistance() const { return m_renderNearClip; }
		void SetNearClipDistance(float nearClip) { m_nearClip = nearClip; }

		/// \name Orthographic specific members
		/// @{
			virtual Rect<float> GetOrthographicRect() const { return m_orthoRect; }
			virtual Rect<float> GetRenderOrthographicRect() const { return m_renderOrthoRect; }
			void SetOrthographicRect(Rect<float> orthoRect) { m_orthoRect = orthoRect; }
		/// @}

		/// \name Perspective specific members
		/// @{
			virtual float GetFOVAngleY() const { return m_fovAngleY; }
			virtual float GetRenderFOVAngleY() const { return m_renderFovAngleY; }
			void SetFOVAngleY(float fovAngle) { m_fovAngleY = fovAngle; }
			virtual float GetAspectMultiplier() const { return m_aspectMultiplier; }
			virtual float GetRenderAspectMultiplier() const { return m_renderAspectMultiplier; }
			void SetAspectMultiplier(float aspectMultiplier) { m_aspectMultiplier = aspectMultiplier; }
		/// @}

		/// Returns a consistently measured distance from this camera to a point (abs in the case of ortho, sq in the case of persp).
		virtual float GetRenderDistanceToPoint(Eigen::Vector3f pos) const;
	/// @}
public:
	static bool HasPhysicsUpdate() { return false; }
	static bool HasCoreUpdate() { return false; }
	static bool HasRenderUpdate() { return false; }
	static bool HasSynchroniseRenderData() { return true; }

private:
	/// \name Core properties
	/// @{
		CameraType m_cameraType;
		float m_farClip;
		float m_nearClip;
		Rect<float> m_orthoRect;
		float m_fovAngleY;
		float m_aspectMultiplier;
	/// @}

	/// \name Render properties
	/// @{
		Eigen::Affine3f m_transform;
		CameraType m_renderCameraType;
		float m_renderFarClip;
		float m_renderNearClip;
		Rect<float> m_renderOrthoRect;
		float m_renderFovAngleY;
		float m_renderAspectMultiplier;
		Eigen::Hyperplane<float, 3> m_renderPlane;
	/// @}
};

