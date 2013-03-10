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
#include "MoveableTexturedQuad.h"

// Project headers
#include "ITexture2D.h"

// Boost headers
#include <boost/assert.hpp>
#include <boost/numeric/conversion/cast.hpp>

// Use Eigen so we don't have to prefix types all the time.
using namespace Eigen;

MoveableTexturedQuad::MoveableTexturedQuad(const ITexture2D* texture) :
	m_texture(texture),
	m_sizeMode(SIZE_ACTUAL_SIZE),
	m_size(1.0f, 1.0f),
	m_baseSize(1.0f, 1.0f),
	m_orientationCenter(Vector2f::Zero()),
	m_position(Vector3f::Zero()),
	m_orientation(Quaternionf::Identity()),
	m_positionOffset(Vector2f::Zero()),
	m_colour(Vector4f::Ones())
{
	BOOST_ASSERT(m_texture != nullptr);
}

MoveableTexturedQuad::~MoveableTexturedQuad()
{}

void MoveableTexturedQuad::InitialiseBaseSize()
{
	// Figure out the base size from the texture.
	switch (m_sizeMode)
	{
	case SIZE_ACTUAL_SIZE:
		m_baseSize = Vector2f(1.0f, 1.0f);
		break;
	case SIZE_START_FROM_ASPECT_RATIO:
		{
			float aspectRatio = boost::numeric_cast<float>(m_texture->GetWidth()) / boost::numeric_cast<float>(m_texture->GetHeight());
			m_baseSize = Vector2f(aspectRatio, 1.0f);
		}
		break;
	case SIZE_START_FROM_TEXTURE_SIZE:
		m_baseSize = Vector2f(boost::numeric_cast<float>(m_texture->GetWidth()), boost::numeric_cast<float>(m_texture->GetHeight()));
		break;
	default:
		// What did you do?!
		BOOST_ASSERT(false);
		break;
	}
}

Affine3f MoveableTexturedQuad::GetTransform() const
{
	Affine3f transform(Affine3f::Identity());
	Vector3f basicScale(m_size(0) * m_baseSize(0), m_size(1) * m_baseSize(1), 1.0f);
	Vector3f orientationCenter(m_orientationCenter(0), m_orientationCenter(1), 0.0f);
	transform.translate(-orientationCenter);
	transform.pretranslate(Vector3f(m_positionOffset(0), m_positionOffset(1), 0.0f));
	transform.prescale(basicScale);
	transform.prerotate(m_orientation);
	transform.pretranslate(orientationCenter);
	transform.pretranslate(m_position);
	return transform;
}