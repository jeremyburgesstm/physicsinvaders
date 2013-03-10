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

#include "GamePhysicsConstants.h"
#include <Box2D/Box2D.h>

const unsigned short c_invaderUnitMask = 
	GamePhysicsConstants::c_defaultPhysicsLayer 
	| GamePhysicsConstants::c_playerBulletLayer 
	| GamePhysicsConstants::c_playerTurretLayer
	| GamePhysicsConstants::c_invaderUnitLayer
	| GamePhysicsConstants::c_floorLayer;
const unsigned short c_playerTurretMask = 
	GamePhysicsConstants::c_defaultPhysicsLayer
	| GamePhysicsConstants::c_invaderUnitLayer
	| GamePhysicsConstants::c_invaderBulletLayer
	| GamePhysicsConstants::c_floorLayer;
const unsigned short c_playerBulletMask =
	GamePhysicsConstants::c_defaultPhysicsLayer
	| GamePhysicsConstants::c_invaderUnitLayer;
const unsigned short c_invaderBulletMask =
	GamePhysicsConstants::c_defaultPhysicsLayer
	| GamePhysicsConstants::c_playerTurretLayer
	| GamePhysicsConstants::c_floorLayer;
const unsigned short c_floorMask =
	GamePhysicsConstants::c_defaultPhysicsLayer
	| GamePhysicsConstants::c_playerTurretLayer
	| GamePhysicsConstants::c_invaderUnitLayer
	| GamePhysicsConstants::c_invaderBulletLayer
	| GamePhysicsConstants::c_playerUnitInvulverabilityLayer;
const unsigned short c_playerUnitInvulnMask = 
	GamePhysicsConstants::c_playerUnitInvulverabilityLayer
	| GamePhysicsConstants::c_defaultPhysicsLayer
	| GamePhysicsConstants::c_floorLayer;
const unsigned short c_invaderWaveMask = 0;

/// Helper to build our filters easily.
static b2Filter CreateFilter(unsigned short layer, unsigned short mask, short group)
{
	b2Filter filter;
	filter.categoryBits = layer;
	filter.maskBits = mask;
	filter.groupIndex = group;
	return filter;
};

b2Filter c_invaderUnitFilter = CreateFilter( GamePhysicsConstants::c_invaderUnitLayer, c_invaderUnitMask, 0 );
b2Filter c_playerTurretFilter = CreateFilter( GamePhysicsConstants::c_playerTurretLayer, c_playerTurretMask, 0 );
b2Filter c_playerBulletFilter = CreateFilter( GamePhysicsConstants::c_playerBulletLayer, c_playerBulletMask, 0 );
b2Filter c_invaderBulletFilter = CreateFilter( GamePhysicsConstants::c_invaderBulletLayer, c_invaderBulletMask, 0 );
b2Filter c_invaderWaveFilter = CreateFilter( GamePhysicsConstants::c_invaderWaveLayer, c_invaderWaveMask, 0 );
b2Filter c_floorFilter = CreateFilter( GamePhysicsConstants::c_floorLayer, c_floorMask, 0 );
b2Filter c_playerInvulnFilter = CreateFilter( GamePhysicsConstants::c_playerUnitInvulverabilityLayer, c_playerUnitInvulnMask, 0 );

const b2Filter& GamePhysicsConstants::GetInvaderUnitFilter()
{
	return c_invaderUnitFilter;
}

const b2Filter& GamePhysicsConstants::GetPlayerUnitFilter()
{
	return c_playerTurretFilter;
}

const b2Filter& GamePhysicsConstants::GetInvaderBulletFilter()
{
	return c_invaderBulletFilter;
}

const b2Filter& GamePhysicsConstants::GetPlayerBulletFilter()
{
	return c_playerBulletFilter;
}

const b2Filter& GamePhysicsConstants::GetInvaderWaveFilter()
{
	return c_invaderWaveFilter;
}

const b2Filter& GamePhysicsConstants::GetFloorFilter()
{
	return c_floorFilter;
}

const b2Filter& GamePhysicsConstants::GetPlayerUnitInvulnFilter()
{
	return c_playerInvulnFilter;
}