#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <mutex>         
#include <string>
#include <thread>
#include <vector>
#include <list>

#include <Windows.h>

#include <glm/glm.hpp>
#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <im.h>

#include "Image.h"
#include "RenderModel.h"
#include "Simulation.h"
#include "AbstractRenderer.h"