#include <iostream>

#define OLC_PGE_APPLICATION
#include "Renderer.h"

using namespace std;

/*

	Add a rain effect in the background
	Change the colours to match better (Platforms green when movable, water shade circles)
	Add explanation on the first level

	Make more levels

	Transition levels by -= dt from the time taken and once at 0,
	replace all of the WorldObjects with the next level.

*/

int main() {
	Renderer r;
	if (r.Construct(1200, 800, 1, 1))
		r.Start();

	return 0;
}