#pragma once

#include "olcPixelGameEngine.h"
#include "WorldObject.h"
#include "GUI.h"

class Level {
public:
	olc::PixelGameEngine* pge;
	std::vector<Circle*>* circles;
	std::vector<Circle*>* collisionCircles;
	std::vector<Platform*>* platforms;
	Goal* goal;
	int* levelIndex;
	int* goalsCounter = new int(0);
	int requiredGoals;
	int initialCirclesAmount;
	float timeLimit, timer = 0;
	TimeDirection timeDirection;
	GUI* gui;

	void update(float dt) {
		UserInteraction();
		UpdateWorldObjects(dt);
		RenderWorldObjects();
	}

	Level(
		olc::PixelGameEngine* _pge,
		std::vector<Circle*>* _circles,
		std::vector<Platform*>* _platforms,
		Goal* _goal,
		int* _levelIndex,
		int _requiredGoals = -1,
		float _timeLimit = 10,
		TimeDirection _tD = TimeDirection::PAUSED
	) {
		pge = _pge;
		circles = _circles;
		platforms = _platforms;
		goal = _goal;
		levelIndex = _levelIndex;
		requiredGoals = _requiredGoals;
		timeLimit = _timeLimit;
		timeDirection = _tD;

		if (_requiredGoals < 1) requiredGoals = circles->size();

		initialCirclesAmount = circles->size();

		gui = new GUI(pge, &timeDirection);
		collisionCircles = new std::vector<Circle*>;

		for (Circle* circle : *circles) circle->timeDirection = &timeDirection;
	}
private:
	void UserInteraction() {
		gui->update(
			levelIndex,
			circles->size(),
			initialCirclesAmount,
			timeLimit - timer,
			requiredGoals
		);
	}

	void UpdateWorldObjects(float dt) {
		if (timeDirection == TimeDirection::FORWARDS) timer += dt;
		if (timeDirection == TimeDirection::BACKWARDS) timer -= dt;

		for (Circle* circle : *circles) {
			circle->updatePhysics(dt, olc::vf2d(pge->ScreenWidth(), pge->ScreenHeight()), *platforms, circles, collisionCircles, &timeLimit, &timer, levelIndex, goal);
			if (circle->goalScored) {
				auto it = find(circles->begin(), circles->end(), circle);
				circles->erase(it);
				delete circle;
				*goalsCounter += 1;
			}
		}
		for (Platform* platform : *platforms) platform->updateInput(pge);
		collisionCircles->clear();
		
		if (*goalsCounter >= requiredGoals) {
			timeDirection = TimeDirection::BACKWARDS;
			if (timer <= -1) *levelIndex += 1;
		}
	}

	void RenderWorldObjects() {
		for (Circle* circle : *circles) circle->draw(pge);
		for (Platform* platform : *platforms) platform->draw(pge, &timeDirection);
		goal->draw(pge);
	}
};