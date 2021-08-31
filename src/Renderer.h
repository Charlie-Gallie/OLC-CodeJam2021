#pragma once

#include <vector>
#include "olcPixelGameEngine.h"
#include "WorldObject.h"
#include "GUI.h"
#include "Level.h"
#include "Rain.h"

class Renderer : public olc::PixelGameEngine {
public:
	std::vector<Level*> levels;
	Rain* rain;
	int* levelIndex = new int(0);

	Renderer() {
		sAppName = "In:Time";
	}

	bool OnUserCreate() override {
		olc::Sprite* goalSprite = new olc::Sprite("./assets/barrel.png");
	
		rain = new Rain(this);

		int circleRadius = 10.0f;
		int platformRadius = 8.0f;

		levels.insert(levels.end(), {
			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() / 2, 200), 10.0f)
				},
				new std::vector<Platform*>{
					
				},
				new Goal(olc::vf2d(ScreenWidth() / 2 - goalSprite->width / 2, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() / 2, 200), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d((ScreenWidth() / 2) - 100, 350), olc::vf2d((ScreenWidth() / 2) + 100, 350), 10)
				},
				new Goal(olc::vf2d(ScreenWidth() / 2 - goalSprite->width / 2, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() / 2, 200), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d((ScreenWidth() / 2) - 100, 350), olc::vf2d((ScreenWidth() / 2) + 100, 350), 10, true),
					new Platform(olc::vf2d((ScreenWidth() / 2) - 100, 400), olc::vf2d((ScreenWidth() / 2) + 100, 400), 10)
				},
				new Goal(olc::vf2d((ScreenWidth() - (goalSprite->width * 2)), ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			)
		});

		return true;
	}

	bool OnUserUpdate(float deltaTime) override {
		Clear(olc::Pixel{ 200, 200, 200 });
		rain->draw(deltaTime);
		if (*levelIndex < levels.size()) levels[*levelIndex]->update(deltaTime);

		return true;
	}
};