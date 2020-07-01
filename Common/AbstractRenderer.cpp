#include "pch.h"

bool AbstractRenderer::wireframe = false;
bool AbstractRenderer::culling = true;
bool AbstractRenderer::shadows = true;
float AbstractRenderer::cameraAngle = 0;
PerformanceCounters AbstractRenderer::PC;