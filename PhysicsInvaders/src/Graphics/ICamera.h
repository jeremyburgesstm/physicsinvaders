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

#include "Core/EigenIncludes.h"
#include "Utility/Rect.h"

/**
 * \class ICamera
 *
 * Basic camera interface.
 */
class ICamera
{
public:
	enum CameraType
	{
		CAMERA_TYPE_ORTHOGRAPHIC,
		CAMERA_TYPE_PERSPECTIVE
	};

public:
	ICamera() {}
	virtual ~ICamera() {}
	
	/// \name Accessors
	/// @{
		/// GetRenders the type of camera.
		virtual CameraType GetRenderCameraType() const = 0;

		/// Returns the position of the camera
		virtual Eigen::Vector3f GetRenderPosition() const = 0;
		/// Returns the focus position of the camera
		virtual Eigen::Vector3f GetRenderFocusPosition() const = 0;
		/// Returns the up vector of the camera
		virtual Eigen::Vector3f GetRenderUp() const = 0;

		/// GetRenders the far clip distance of the camera
		virtual float GetRenderFarClipDistance() const = 0;
		/// GetRenders the near clip distance of the camera
		virtual float GetRenderNearClipDistance() const = 0;

		/// \name Orthographic specific members
		/// @{
			virtual Rect<float> GetRenderOrthographicRect() const = 0;
		/// @}

		/// \name Perspective specific members
		/// @{
			virtual float GetRenderFOVAngleY() const = 0;
			virtual float GetRenderAspectMultiplier() const = 0;
		/// @}

		virtual float GetRenderDistanceToPoint(Eigen::Vector3f pos) const = 0;
	/// @}
};