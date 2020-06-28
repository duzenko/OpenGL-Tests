#include "pch.h"

Images* AbstractRenderer::images;
bool AbstractRenderer::wireframe = false;
bool AbstractRenderer::culling = true;
float AbstractRenderer::cameraAngle = 0;
PerformanceCounters AbstractRenderer::PC;