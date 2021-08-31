#pragma once

#include "olcPixelGameEngine.h"
#include "WorldObject.h"

class Button {
public:
	std::string fileNameWithoutSuffix, directory;
	olc::vf2d position;
	TimeDirection* timeDirection,  activeTimeDirection;
	bool selected;

	Button(
		std::string _fNWS,
		std::string _dir,
		olc::vf2d _pos,
		TimeDirection* _tD,
		TimeDirection _aTD,
		bool _sel = false
	) {
		fileNameWithoutSuffix = _fNWS;
		directory = _dir;
		position = _pos;
		selected = _sel;
		timeDirection = _tD;
		activeTimeDirection = _aTD;

		decalSelected = new olc::Decal(new olc::Sprite(directory + fileNameWithoutSuffix + "Selected.png"));
		decalUnselected = new olc::Decal(new olc::Sprite(directory + fileNameWithoutSuffix + "Unselected.png"));
	}

	olc::Decal* getDecal() {
		if (*timeDirection != activeTimeDirection) selected = false;
		else selected = true;
		if (selected) return decalSelected;
		return decalUnselected;
	}

	void select() {
		selected = true;
		*timeDirection = activeTimeDirection;
	}

private:
	olc::Decal* decalSelected;
	olc::Decal* decalUnselected;
};

class GUI {
public:
	olc::PixelGameEngine* pge;
	TimeDirection* timeDirection;
	std::vector<Button*> buttons;
	olc::Pixel textColour = olc::DARK_BLUE;
	int textScale = 2;

	void update(
		int levelIndex,
		int circlesRemaining,
		int initialCircleAmount,
		float timeRemaining,
		int circlesRequired
	) {
		for (Button* button : buttons) {
			olc::Decal* d = button->getDecal();
			pge->DrawDecal(button->position, d);
			if (pge->GetMouse(0).bPressed) {
				olc::vf2d mousePos = pge->GetMousePos();
				if (PointInQuad(mousePos, button->position, olc::vf2d(d->sprite->width, d->sprite->height))) {
					button->select();
				}
			}
		}

		pge->DrawString(olc::vf2d(10, 65), "Time Remaining: " + std::to_string((int)timeRemaining), textColour, textScale);
		pge->DrawString(olc::vf2d(10, 85), "Required: " + std::to_string(circlesRequired), textColour, textScale);
		pge->DrawString(olc::vf2d(10, 105), "Scored: " + std::to_string(initialCircleAmount - circlesRemaining) + "/" + std::to_string(initialCircleAmount), textColour, textScale);
		pge->DrawString(olc::vf2d(10, 125), "Level: " + std::to_string(levelIndex), textColour, textScale);

		if (levelIndex == 0) {
			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 3, 400), "Use the options at the top", textColour, textScale);
			pge->DrawString(olc::vf2d((pge->ScreenWidth() / 4) + 20, 420), "left to alter the direction of time", textColour, textScale);			
		}

		if (levelIndex == 1) {
			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 3 + 20, 400), "You can move platforms", textColour, textScale);
			pge->DrawString(olc::vf2d((pge->ScreenWidth() / 3 + 40) + 20, 420), "using the anchors", textColour, textScale);

			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 6, 460), "Platforms are locked while time is going forwards", textColour, textScale);
			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 3 + 40, 500), "Platforms are porous", textColour, textScale);
		}

		if (levelIndex == 2) {
			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 4 + 60, 450), "Grey platforms cannot be moved", textColour, textScale);

			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 4 + 20, 490), "Water droplets have surface tension", textColour, textScale);
			pge->DrawString(olc::vf2d(pge->ScreenWidth() / 3 + 105, 510), "on platforms.", textColour, textScale);
		}
	}
	
	GUI(
		olc::PixelGameEngine* _pge,
		TimeDirection* _tD
	) {
		pge = _pge;
		timeDirection = _tD;

		buttons.insert(buttons.end(), {
			new Button("btnBackwards", "./assets/", olc::vf2d(10, 10), _tD, TimeDirection::BACKWARDS),
			new Button("btnPaused", "./assets/", olc::vf2d(60, 10), _tD, TimeDirection::PAUSED),
			new Button("btnForwards", "./assets/", olc::vf2d(110, 10), _tD, TimeDirection::FORWARDS)
		});
	}
private:
	std::string suffix = "Unselected";

	bool PointInQuad(olc::vf2d point, olc::vf2d quad, olc::vf2d quadSize) {
		if (
			point.x >= quad.x &&
			point.x <= quad.x + quadSize.x &&
			point.y >= quad.y &&
			point.y <= quad.y + quadSize.y
			) return true;
		return false;
	}
};