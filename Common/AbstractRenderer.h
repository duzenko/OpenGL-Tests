#pragma once

struct PerformanceCounters {
	int drawCalls;
	int drawTriangles;
	int textureSwitches;
	int shaderSwitches;
};

struct AbstractRenderer {

protected:
	std::vector<DrawSurface*> drawSurfaces;
	void ListSurfaces();
	virtual void DeformSurface(DrawSurface &surf);

public:
	static void MouseInput( double xpos, double ypos );

	virtual void Render( Simulation& simulation ) = 0;

	static bool ambient;
	static bool culling;
	static bool lighting;
	static bool shadows;
	static bool wireframe;
	static float cameraAngleX;
	static float cameraAngleY;
	static float aspectRatio;

	static PerformanceCounters PC;
};

