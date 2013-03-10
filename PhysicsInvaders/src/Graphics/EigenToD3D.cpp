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

#include "EigenToD3D.h"

XMMATRIX EigenToD3D::EigenAffineToD3DMatrix(const Eigen::Affine3f& affine)
{
	XMMATRIX matrix;
	EigenAffineToD3DMatrix(affine, matrix);
	return matrix;
}

Eigen::Affine3f EigenToD3D::D3DMatrixToEigenAffine(const XMMATRIX& matrix)
{
	Eigen::Affine3f affine;
	D3DMatrixToEigenAffine(matrix, affine);
	return affine;
}

void EigenToD3D::EigenAffineToD3DMatrix(const Eigen::Affine3f& affine, XMMATRIX& matrix)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			matrix(j, i) = affine(i, j);
		}
	}
}

void EigenToD3D::D3DMatrixToEigenAffine(const XMMATRIX& matrix, Eigen::Affine3f& affine)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			affine(j, i) = matrix(i, j);
		}
	}
}

XMFLOAT3 EigenToD3D::EigenVector3fToD3DVector3(const Eigen::Vector3f& vector)
{
	return XMFLOAT3(vector[0], vector[1], vector[2]);
}

XMFLOAT4 EigenToD3D::EigenVector4fToD3DVector4(const Eigen::Vector4f& vector)
{
	return XMFLOAT4(vector[0], vector[1], vector[2], vector[3]);
}