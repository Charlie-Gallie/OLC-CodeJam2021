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
	Level* currentLevel = nullptr;

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
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() - 100, 50), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d(10, ScreenHeight() - goalSprite->height * 1.5f), olc::vf2d(280, ScreenHeight() - goalSprite->height * 1.5f), 10, true),
					new Platform(olc::vf2d((ScreenWidth() - 20), 100), olc::vf2d((ScreenWidth() - 180), 100), 10)
				},
				new Goal(olc::vf2d(goalSprite->width / 1.5f, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(300, 150), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d(10, ScreenHeight() * 0.4f), olc::vf2d(ScreenWidth() * 0.6f, ScreenHeight() * 0.4f), 10, true),
					new Platform(olc::vf2d(ScreenWidth() * 0.6f + 20, ScreenHeight() * 0.4f), olc::vf2d(ScreenWidth() - 10, ScreenHeight() * 0.4f), 10),
					new Platform(olc::vf2d(ScreenWidth() * 0.6f + 20, ScreenHeight() * 0.4f + 40), olc::vf2d(ScreenWidth() - 10, ScreenHeight() * 0.4f + 40), 10)
				},
				new Goal(olc::vf2d(ScreenWidth() / 2 - goalSprite->width / 2, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() * 0.85f, ScreenHeight() * 0.1f), 10.0f),
					new Circle(olc::vf2d(ScreenWidth() * 0.8f, ScreenHeight() * 0.1f), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d(10, ScreenHeight() - goalSprite->height * 2), olc::vf2d(goalSprite->width * 2, ScreenHeight() - goalSprite->height * 2), 10, true),
					new Platform(olc::vf2d(ScreenWidth() * 0.9f, ScreenHeight() * 0.1f), olc::vf2d(ScreenWidth() * 0.9f, ScreenHeight() * 0.3f), 10)
				},
				new Goal(olc::vf2d(goalSprite->width / 2, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),

			new Level(
				this,
				new std::vector<Circle*>{
					new Circle(olc::vf2d(ScreenWidth() * 0.15f, 150), 10.0f),
					new Circle(olc::vf2d(ScreenWidth() * 0.85f, 150), 10.0f)
				},
				new std::vector<Platform*>{
					new Platform(olc::vf2d(ScreenWidth() * 0.35f, ScreenHeight() - goalSprite->height * 1.5f), olc::vf2d(ScreenWidth() * 0.65f, ScreenHeight() - goalSprite->height * 1.5f), 10, true),
					new Platform(olc::vf2d(ScreenWidth() * 0.35f, ScreenHeight() - goalSprite->height * 2.0f), olc::vf2d(ScreenWidth() * 0.65f, ScreenHeight() - goalSprite->height * 2.0f), 10),
					new Platform(olc::vf2d(ScreenWidth() * 0.35f, ScreenHeight() - goalSprite->height * 2.5f), olc::vf2d(ScreenWidth() * 0.65f, ScreenHeight() - goalSprite->height * 2.5f), 10)
				},
				new Goal(olc::vf2d(ScreenWidth() / 2 - goalSprite->width / 2, ScreenHeight() - goalSprite->height), goalSprite),
				levelIndex,
				-1,
				5
			),
		});

		currentLevel = levels[*levelIndex];

		return true;
	}

	bool OnUserUpdate(float deltaTime) override {
		Clear(olc::Pixel{ 200, 200, 200 });
		rain->draw(deltaTime);

		/* 
		 * The reset button doesn't work - Have a go fixing it if you want
		 * 
		 * What I'm doing is, if the 'levelIndex' is a negative, that indicates the reset
		 * button has been pressed, so I get the value of 'currentLevel' to a new instance
		 * of the level from the 'levels' vector, and set 'levelIndex' back to a positive.
		 * 
		 * I check if the button has been pressed in GUI.h around line 78.
		 * 
		 * I know how bad this is, but this entire game is just one massive bodge.
		*/

		if (*currentLevel->levelIndex < 0) {
			*levelIndex = -(*levelIndex);
			currentLevel = new Level(*levels[*levelIndex]);
		}
	
		if (*levelIndex != *currentLevel->levelIndex) currentLevel = new Level(*levels[*levelIndex]);
		if (*levelIndex < levels.size()) currentLevel = levels[*levelIndex];
		currentLevel->update(deltaTime);

		/*
		* Delete the levels which have been played
		*/

		return true;
	}
};