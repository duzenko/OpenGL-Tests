#pragma once

#include <filesystem>
#include <list>
#include <map>
#include <memory>
#include <mutex>         
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include <Windows.h>

#define GLM_FORCE_INLINE
#define GLM_FORCE_XYZW_ONLY
#include <glm/glm.hpp>
#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

#include <im.h>

#include "AbstractImage.h"
#include "RenderModel.h"
#include "Simulation.h"
#include "AbstractRenderer.h"
#include "Util.h"