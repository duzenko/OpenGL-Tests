#pragma once

struct PerformanceCounters {
	int drawCalls;
	int drawTriangles;
	int textureSwitches;
};

struct AbstractRenderer {
public:
	TerrainModel terrain;

	virtual void Render( Simulation& simulation ) = 0;

	static bool wireframe;
	static bool culling;
	static float cameraAngle;

	static PerformanceCounters PC;
};

