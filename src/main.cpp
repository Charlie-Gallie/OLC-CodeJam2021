#include <iostream>

#define OLC_PGE_APPLICATION
#include "Renderer.h"

using namespace std;

/*
* This game is programmed really bad.
* For context, I had only a few hours over the 9 days to create all of this and I didn't plan for a lot of features therefore, many
* elements of gameplay are just forced in, and some parts don't even work, such as everything listed below.
* 
* Some known problems;
*	- The reset button doesn't work, refer to Render.h around line 150 to see why.
* 
*	- The "ball" speed if affected by the frame rate because I couldn't modulate it by fElapsedTime, purely because I'm bad,
*	  refer to WorldObject.h aroubd line 195 to see why. (I call 'fElapsedTime' 'deltaTime' or just 'dt', it's the same thing)
* 
*	- There's probably subtle memory leaks - The memory usage in the profiler is pretty strange and I don't clean up new objects,
*	  nor do I use smart pointers.
* 
*	- The barrel decal is rendered above the platform anchors.
* 
*	- The rain is affected by the frame rate - It looks good at ~150fps, but when at ~300fps, the rain comes down in bursts.
*	  Adding to the rain, I don't delete the rainDrop object, but I think it's cleared up anyway.
* 
*	- Nothing is done when the player finishes the last level, I only check that 'levelIndex' isn't larger than the amount of elements
*	  in the vector storing all the levels - every frame.
* 
*	- Plus more poorly designed elements. Primarily, please just know that I don't consider this a good demonstration of quality C++
	  nor my skills.
*/

int main() {
	Renderer r;
	if (r.Construct(1200, 800, 1, 1))
		r.Start();

	return 0;
}
