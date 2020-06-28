#pragma once

#define GLM_FORCE_XYZW_ONLY

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <mutex>         
#include <thread>

//#include <windows.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <im.h>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad.h"

#include "Image.h"
#include "RenderModel.h"

#include "Simulation.h"
#include "Renderer.h"