#pragma once

#include <vector>
#include <algorithm>
#include "olcPixelGameEngine.h"

enum class Edge {
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	NONE
};

enum class TimeDirection {
	FORWARDS,
	PAUSED,
	BACKWARDS
};

class TimeSnapshot {
public:
	olc::vf2d
		position,
		velocity;

	TimeSnapshot(olc::vf2d _pos, olc::vf2d _vel) {
		position = _pos;
		velocity = _vel;
	}

	TimeSnapshot() {}

	bool operator==(TimeSnapshot& right) const {
		if (velocity != right.velocity) return false;
		if (position != right.position) return false;
		return true;
	}

	bool operator!=(TimeSnapshot& right) const {
		if (velocity == right.velocity) return false;
		if (position == right.position) return false;
		return true;
	}
};

class Platform {
public:
	olc::vf2d
		start,
		end;
	float radius;
	olc::Pixel colour;
	bool
		locked,
		permLocked;

	void draw(olc::PixelGameEngine* pge, TimeDirection* timeDirection) {
		if (*timeDirection == TimeDirection::FORWARDS || *timeDirection == TimeDirection::BACKWARDS) locked = true;
		else locked = false;
		if (locked) colour = olc::DARK_RED;
		else colour = olc::DARK_CYAN;
		if (permLocked) colour = olc::DARK_GREY;

		pge->FillCircle(start, radius, colour);
		pge->FillCircle(end, radius, colour);

		olc::vf2d normal = {
			-(end.y - start.y),
			(end.x - start.x)
		};

		float distance = sqrt(
			normal.x * normal.x + 
			normal.y * normal.y
		);

		normal /= distance;

		pge->DrawLine(
			(start.x + normal.x * radius),
			(start.y + normal.y * radius),
			(end.x + normal.x * radius),
			(end.y + normal.y * radius),
			colour
		);

		pge->DrawLine(
			(start.x - normal.x * radius),
			(start.y - normal.y * radius),
			(end.x - normal.x * radius),
			(end.y - normal.y * radius),
			colour
		);
	}

	void updateInput(olc::PixelGameEngine* pge) {
		if (permLocked || locked) return;
		olc::vf2d mousePosition = pge->GetMousePos();

		if (pge->GetMouse(0).bPressed) {
			if (selected != nullptr) {
				*selected = mousePosition;
			}
			else if (PointInCircle(mousePosition, start, radius)) selected = &start;
			else if (PointInCircle(mousePosition, end, radius)) selected = &end;
		}
		if (pge->GetMouse(0).bHeld) {
			if (selected != nullptr) *selected = mousePosition;
		}
		if (pge->GetMouse(0).bReleased) selected = nullptr;
	}
	Platform(olc::vf2d _start, olc::vf2d _end, float _rad, bool _permLocked = false, bool _locked = false, olc::Pixel _col = olc::DARK_GREEN) {
		start = _start;
		end = _end;
		radius = _rad;
		colour = _col;
		locked = _locked;
		permLocked = _permLocked;
	}
private:
	olc::vf2d* selected = nullptr;

	bool PointInCircle(olc::vf2d point, olc::vf2d circle, float circleRadius) {
		return fabs(
			(circle.x - point.x) * (circle.x - point.x) +
			(circle.y - point.y) * (circle.y - point.y)
			< circleRadius * circleRadius
		);
	}
};

class Goal {
public:
	olc::vf2d
		position,
		scale;
	olc::Sprite* sprite;
	olc::Decal* decal;

	void draw(olc::PixelGameEngine* pge) {
		pge->SetPixelMode(olc::Pixel::MASK);
		pge->DrawDecal(position, decal, scale);
	}

	Goal(
		olc::vf2d _position,
		olc::Sprite* _sprite,
		olc::vf2d _scale = olc::vf2d(1, 1)
	) {
		position = _position;
		sprite = _sprite;
		scale = _scale;
		decal = new olc::Decal(sprite);
	}
};

class Circle {
public:
	olc::vf2d
		position,
		nextPosition,
		acceleration,
		velocity;
	float
		radius,
		rewinding = false,
		mass;
	float* timer;
	olc::Pixel colour;
	TimeDirection* timeDirection;
	bool goalScored = false;

	void draw(olc::PixelGameEngine* pge) {
		pge->FillCircle(position, radius, colour);
	}

	void updatePhysics(
		float dt,
		olc::vf2d Screen,
		std::vector<Platform*> platforms,
		std::vector<Circle*>* circles,
		std::vector<Circle*>* collisionCircles,
		float* timeLimit,
		float* _timer,
		int* levelIndex,
		Goal* goal
	) {
		timer = _timer;
		if (*timeDirection == TimeDirection::FORWARDS) {
			if (*timer >= *timeLimit) *timeDirection = TimeDirection::BACKWARDS; 
			if (PointInQuad(position, goal->position, olc::vf2d(goal->sprite->width, goal->sprite->height))) goalScored = true;
			
			colour = olc::DARK_BLUE;
			acceleration = (-velocity * 0.8f) * dt;
			velocity += acceleration * dt;
			nextPosition = position + (velocity * dt);

			for (Platform* platform : platforms) {
				olc::vf2d line1 = {
					platform->end.x - platform->start.x,
					platform->end.y - platform->start.y,
				};
				olc::vf2d line2 = {
					position.x - platform->start.x,
					position.y - platform->start.y
				};

				float platformLength = line1.x * line1.x + line1.y * line1.y;
				float t = std::fmax(0, std::fmin(platformLength, (line1.x * line2.x + line1.y * line2.y))) / platformLength;
				olc::vf2d closestPoint = {
					platform->start.x + t * line1.x,
					platform->start.y + t * line1.y
				};

				float distance = sqrtf(
					(position.x - closestPoint.x) * (position.x - closestPoint.x) +
					(position.y - closestPoint.y) * (position.y - closestPoint.y)
				);

				if (distance < radius + platform->radius) {
					//velocity.y = setNegative(velocity.y);

					Circle* collisionCircle = new Circle(closestPoint, platform->radius);
					collisionCircle->velocity = -velocity;

					collisionCircles->push_back(collisionCircle);
					collisions.push_back({ this, collisionCircle });

					float overlap = 1.0f * (distance - radius - platform->radius);
					position.x -= overlap * (position.x - collisionCircle->position.x) / distance;
					position.y -= overlap * (position.y - collisionCircle->position.y) / distance;
				}
			}

			for (Circle* subject : *circles) {
				for (Circle* target : *circles) {
					if (subject != target) {
						if (CirclesOverlap(*subject, *target)) {
							collisions.push_back({ subject, target });

							float distance = sqrtf(
								(subject->position.x - target->position.x) * (subject->position.x - target->position.x) +
								(subject->position.y - target->position.y) * (subject->position.y - target->position.y)
							);
							float overlap = 0.5f * (distance - subject->radius - target->radius);

							subject->position.x -= overlap * (subject->position.x - target->position.x) / distance;
							subject->position.y -= overlap * (subject->position.y - target->position.y) / distance;
						}
					}
				}
			}

			float efficiency = 1.0f;
			for (auto pair : collisions) {
				Circle* b1 = pair.first;
				Circle* b2 = pair.second;
				float distance = sqrtf(
					(b1->position.x - b2->position.x) * (b1->position.x - b2->position.x) +
					(b1->position.y - b2->position.y) * (b1->position.y - b2->position.y)
				);

				olc::vf2d normal = {
					(b2->position.x - b1->position.x) / distance,
					(b2->position.y - b1->position.y) / distance
				};
				olc::vf2d tangent = {
					-normal.y,
					normal.x
				};
				float dotProdTan1 = b1->velocity.x * tangent.x + b1->velocity.y * tangent.y;
				float dotProdTan2 = b2->velocity.x * tangent.x + b2->velocity.y * tangent.y;

				float dotProdNorm1 = b1->velocity.x * normal.x + b1->velocity.y * normal.y;
				float dotProdNorm2 = b2->velocity.x * normal.x + b2->velocity.y * normal.y;

				float momentum1 = efficiency * (dotProdNorm1 * (b1->mass - b2->mass) + 2.0f * b2->mass * dotProdNorm2) / (b1->mass + b2->mass);
				float momentum2 = efficiency * (dotProdNorm2 * (b2->mass - b1->mass) + 2.0f * b1->mass * dotProdNorm1) / (b1->mass + b2->mass);

				b1->velocity.x = tangent.x * dotProdTan1 + normal.x * momentum1;
				b1->velocity.y = tangent.y * dotProdTan1 + normal.y * momentum1;

				b2->velocity.x = tangent.x * dotProdTan2 + normal.x * momentum2;
				b2->velocity.y = tangent.y * dotProdTan2 + normal.y * momentum2;
			}
			collisions.clear();

			Edge WithinScreenCheck = WithinScreen(nextPosition, Screen);
			if (WithinScreenCheck == Edge::NONE) velocity.y += 9.8;

			switch (WithinScreenCheck) {
			case Edge::TOP: velocity.y = setPositive(velocity.y); break;
			case Edge::BOTTOM: velocity.y = setNegative(velocity.y); break;
			case Edge::LEFT: velocity.x = setPositive(velocity.x); break;
			case Edge::RIGHT: velocity.x = setNegative(velocity.x); break;
			}

			TimeSnapshot* snapshot = new TimeSnapshot(position, velocity);
			if (timeSnapshots.size() > 0) {
				if (*timeSnapshots.back() != *snapshot) timeSnapshots.push_back(snapshot);
			}
			else timeSnapshots.push_back(snapshot);
			position = nextPosition;
		}
		if (*timeDirection == TimeDirection::BACKWARDS && timeSnapshots.size() > 0) {
			colour = olc::DARK_RED;
			position = timeSnapshots.back()->position;
			velocity = timeSnapshots.back()->velocity;
			timeSnapshots.pop_back();
		}
		else if (timeSnapshots.size() <= 0) {
			colour = olc::DARK_BLUE;
			*timeDirection = TimeDirection::PAUSED;
			*_timer = 0;
		}
		if (*timeDirection == TimeDirection::FORWARDS) colour = olc::DARK_BLUE;
	}

	Circle(
		olc::vf2d _pos,
		float _rad,
		TimeDirection* _tD = nullptr,
		olc::Pixel _col = olc::DARK_BLUE
	) {
		position = _pos;
		radius = _rad;
		colour = _col;
		timeDirection = _tD;
		mass = (3.14159f * radius) * (3.14159f * radius);
	}
private:
	bool PointInQuad(olc::vf2d point, olc::vf2d quad, olc::vf2d quadSize) {
		if (
			point.x >= quad.x &&
			point.x <= quad.x + quadSize.x &&
			point.y >= quad.y &&
			point.y <= quad.y + quadSize.y
			) return true;
		return false;
	}

	std::vector<TimeSnapshot*> timeSnapshots;
	std::vector<std::pair<Circle*, Circle*>> collisions;

	Edge WithinScreen(olc::vf2d position, olc::vf2d Screen) {
		if (position.y - radius < 0) return Edge::TOP;
		if (position.y + radius > Screen.y) return Edge::BOTTOM;
		if (position.x - radius < 0) return Edge::LEFT;
		if (position.x + radius > Screen.x) return Edge::RIGHT;
		return Edge::NONE;
	}

	bool CirclesOverlap(Circle circle1, Circle circle2) {
		return fabs(
			(circle1.position.x - circle2.position.x) * (circle1.position.x - circle2.position.x) +
			(circle1.position.y - circle2.position.y) * (circle1.position.y - circle2.position.y)
			<= (circle1.radius + circle2.radius) * (circle1.radius + circle2.radius)
		);
	}

	float setPositive(float value) {
		return value > 0 ? value : -value;
	}

	float setNegative(float value) {
		return value > 0 ? -value : value;
	}
};