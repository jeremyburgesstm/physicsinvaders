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

class ITexture2D;

// Eigen types.
#include <Core\EigenIncludes.h>

/**
 * \class MoveableTexturedQuad
 * 
 * Probably doesn't need to be Direct3D specific. Aggregates a Texture2D & a transform.
 */
class MoveableTexturedQuad 
{
public:
	enum SizeMode
	{
		SIZE_ACTUAL_SIZE,
		SIZE_START_FROM_ASPECT_RATIO,
		SIZE_START_FROM_TEXTURE_SIZE
	};

	// This Macro allows us to have Eigen types as members of this class (creates an aligned new).
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

public:
	MoveableTexturedQuad(const ITexture2D* texture);
	~MoveableTexturedQuad();

	/// \name Accessors & Mutators
	/// @{
		inline void SetSizeMode(SizeMode sizeMode) 
		{ 
			m_sizeMode = sizeMode; 
			InitialiseBaseSize(); 
		}
		inline SizeMode GetSizeMode() const { return m_sizeMode; }

		inline void SetSizeScale(const Eigen::Vector2f& sizeScale) { m_size = sizeScale; }
		inline const Eigen::Vector2f& GetSizeScale() const { return m_size; }

		inline void SetPosition(const Eigen::Vector3f& position) { m_position = position; }
		inline const Eigen::Vector3f& GetPosition() const { return m_position; }

		inline void SetOrientation(const Eigen::Quaternionf& orientation) { m_orientation = orientation; }
		inline const Eigen::Quaternionf& GetOrientation() { return m_orientation; }

		inline void SetOrientationCenter(const Eigen::Vector2f& orientationCenter) { m_orientationCenter = orientationCenter; }
		inline const Eigen::Vector2f& GetOrientationCenter() const { return m_orientationCenter; }

		inline void SetPositionOffset(const Eigen::Vector2f& positionOffset) { m_positionOffset = positionOffset; }
		inline const Eigen::Vector2f& GetPositionOffset() const { return m_positionOffset; }

		inline void SetColour(const Eigen::Vector4f& colour) { m_colour = colour; }
		inline const Eigen::Vector4f& GetColour() const { return m_colour; }

		Eigen::Affine3f GetTransform() const;

		inline const ITexture2D* GetTexture() const { return m_texture; }
		void SetTexture(const ITexture2D* texture) { m_texture = texture; }
	/// @}

private:
	void InitialiseBaseSize();

private:
	const ITexture2D* m_texture;
	SizeMode m_sizeMode;
	Eigen::Vector2f m_size;
	Eigen::Vector2f m_baseSize;
	Eigen::Vector2f m_orientationCenter;
	Eigen::Vector3f m_position;
	Eigen::Quaternionf m_orientation;
	Eigen::Vector2f m_positionOffset;
	Eigen::Vector4f m_colour;
};