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

struct b2Filter;

namespace GamePhysicsConstants
{
	const unsigned short c_defaultPhysicsLayer = 1 << 0;
	const unsigned short c_playerTurretLayer = 1 << 1;
	const unsigned short c_invaderUnitLayer = 1 << 2;
	const unsigned short c_playerBulletLayer = 1 << 3;
	const unsigned short c_invaderBulletLayer = 1 << 4;
	const unsigned short c_invaderWaveLayer = 1 << 5;
	const unsigned short c_floorLayer = 1 << 6;
	const unsigned short c_playerUnitInvulverabilityLayer = 1 << 7;

	const b2Filter& GetInvaderUnitFilter();
	const b2Filter& GetPlayerUnitFilter();
	const b2Filter& GetInvaderBulletFilter();
	const b2Filter& GetPlayerBulletFilter();
	const b2Filter& GetInvaderWaveFilter();
	const b2Filter& GetFloorFilter();
	const b2Filter& GetPlayerUnitInvulnFilter();
};