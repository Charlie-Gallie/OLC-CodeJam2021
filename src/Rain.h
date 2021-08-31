#pragma once

#include "olcPixelGameEngine.h"

class RainDrop {
public:
	olc::vf2d position;
	float speed;
	int size;

	RainDrop(olc::vf2d _pos, int _size, float _speed) {
		position = _pos;
		size = _size;
		speed = _speed;
	}

	void update(float dt) {
		position.y += speed * dt;
	}
};

class Rain {
public:
	olc::PixelGameEngine* pge;
	olc::Pixel rainColour = olc::Pixel { 20, 146, 255 };
	//olc::Pixel rainColour = olc::Pixel{ 0, 0, 0 };
	std::vector<RainDrop*> rainDrops;
	float timer = 0;
	int dropsPerSecondLimit = 20;
	int dropsThisSecond = 0;


	Rain(olc::PixelGameEngine* _pge) {
		pge = _pge;
	}

	void draw(float dt) {
		if (timer > 1) {
			timer = 0; dropsThisSecond = 0;
		}
		timer += dt;

		int val = std::rand() % (int)10000 * dt;
		if (timer < 1 && dropsPerSecondLimit > dropsThisSecond && val == 1) {
			if (rainDrops.size() < 1000) rainDrops.push_back(
				new RainDrop({
					olc::vf2d(std::rand() % pge->ScreenWidth(), 0),
					std::rand() % 3,
					500
					})
			);
			dropsThisSecond++;
		}

		/*
			Under development - don't take any of this code seriously...
			Especially the "nullptr" stuff below
		*/

		for (int index = 0; index < rainDrops.size(); index++) {
			if (rainDrops[index] != nullptr) {
				pge->DrawCircle(rainDrops[index]->position, rainDrops[index]->size, rainColour);
				rainDrops[index]->update(dt);

				if (rainDrops[index]->position.y > pge->ScreenHeight()) {
					rainDrops[index] = nullptr;
				}
			}
		}
	}
};