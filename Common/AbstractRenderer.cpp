#include "pch.h"

bool AbstractRenderer::ambient = true;
bool AbstractRenderer::culling = true;
bool AbstractRenderer::lighting = true;
bool AbstractRenderer::shadows = true;
bool AbstractRenderer::wireframe = false;
float AbstractRenderer::cameraAngle = 0;
PerformanceCounters AbstractRenderer::PC;