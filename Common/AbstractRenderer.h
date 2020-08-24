#pragma once

struct PerformanceCounters {
	int drawCalls;
	int drawTriangles;
	int textureSwitches;
	int shaderSwitches;
};

struct RendererParams {
	static inline bool ambient = true;
	static inline bool culling = true;
	static inline bool lighting = true;
	static inline bool shadows = true;
	static inline bool deforms = true;
	static inline bool wireframe = false;
	static inline float cameraAngleX = 0;
	static inline float cameraAngleY = 0;
	static inline float aspectRatio = 1;

	static inline PerformanceCounters PC;
};

struct AbstractRenderer {
	static void MouseInput( double xpos, double ypos );


	AbstractRenderer() {
	}
	~AbstractRenderer() {
		delete abstractImages;
	}
	virtual void Render( Simulation& simulation ) = 0;

protected:
	std::vector<DrawSurface*> drawSurfaces;
	void ListSurfaces();

	virtual void DeformSurface( DrawSurface& surf );
};

