#include <iostream>

#define OLC_PGE_APPLICATION
#include "Renderer.h"

using namespace std;

int main() {
	Renderer r;
	if (r.Construct(1200, 800, 1, 1))
		r.Start();

	return 0;
}
