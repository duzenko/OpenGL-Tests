#pragma once

struct PerformanceCounters {
	int drawCalls;
	int drawTriangles;
	int textureSwitches;
};

class AbstractRenderer {
public:
	static Images* images;

	TerrainModel terrain;

	void Render( Simulation& simulation );

	static bool wireframe;
	static bool culling;
	static float cameraAngle;

	static PerformanceCounters PC;

	AbstractRenderer() {
		images = new Images();
	}
};

