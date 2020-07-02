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

	static bool ambient;
	static bool culling;
	static bool lighting;
	static bool shadows;
	static bool wireframe;
	static float cameraAngle;

	static PerformanceCounters PC;
};

