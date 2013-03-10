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

class Box2DBodyComponent;
class GameContext;
class StateOfTheGame;
class Invader;
class InvaderWaveDefinition;
class InvaderDefinition;
class ITexture2D;
class ICamera;
class b2Body;
namespace ComponentModel
{
	class Entity;
};

namespace ShouldBeDataDriven
{	
	static const char c_cameraName[] = "Camera";

	ICamera* CreateCamera(const GameContext& context);
	void CreatePlayArea(const GameContext& context);
	void CreateTurret(const GameContext& context, const StateOfTheGame& state);
	InvaderWaveDefinition* CreateBasicInvaderWaveDef(float difficulty, int rowCount = 3);
	InvaderWaveDefinition* CreateDrapesInvaderWaveDef(float difficulty, bool hard = false);
	InvaderWaveDefinition* CreateDiagonalInvaderWaveDef(float difficulty);;
	void CreateInvaderWave(const GameContext& context, InvaderWaveDefinition* def);
	ComponentModel::Entity* CreateInvader0(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def);
	ComponentModel::Entity* CreateInvader1(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def);
	ComponentModel::Entity* CreateInvader2(const GameContext& context, Box2DBodyComponent* invaderWave, const InvaderDefinition& def);
	ComponentModel::Entity* CreateInvader(const GameContext& context, Box2DBodyComponent* invaderWave, const ITexture2D* texture, const InvaderDefinition& def);
	ComponentModel::Entity* CreateBullet(b2Body* owner, const GameContext& context, bool player);
	void LevelUpInvader(ComponentModel::Entity* invaderEntity, Invader* invader, const GameContext& context);
}