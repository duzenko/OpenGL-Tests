#pragma once

#define _USE_MATH_DEFINES // for C++
#include <math.h>

#include <glm/ext/matrix_float4x4.hpp> 
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/trigonometric.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Simulation.h"
#include "Image.h"

struct SurfaceVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

struct SphereRenderModel {
	int strips;
	int vertsPerStrip;
	SurfaceVertex* vertices;

    SphereRenderModel( int subdiv ) {
        const float r = 1;
        int i, j;
        int lats = subdiv, longs = 2 * subdiv;
        strips = lats;
        vertsPerStrip = longs * 2 + 2;
        vertices = new SurfaceVertex[strips * vertsPerStrip];
        SurfaceVertex* vertPointer = vertices;

        for ( i = 0; i < lats; i++ ) {
            float t1 = (float) i / lats;
            double lat0 = M_PI * ( -0.5 + t1 );
            double y0 = sin( lat0 );
            double yr0 = cos( lat0 );

            float t2 = (float) ( i + 1 ) / lats;
            double lat1 = M_PI * ( -0.5 + t2 );
            double y1 = sin( lat1 );
            double yr1 = cos( lat1 );

            for ( j = 0; j <= longs; j++ ) {
                float s = (float) j / longs;
                double lng = 2 * M_PI * s;
                double x = -sin( lng );
                double z = -cos( lng );

                vertPointer->texCoord = glm::vec2( s, t2 );
                vertPointer->normal = glm::vec3( x * yr1, y1, z * yr1 );
                vertPointer->position = vertPointer->normal * r;
                vertPointer++;

                vertPointer->texCoord = glm::vec2( s, t1 );
                vertPointer->normal = glm::vec3( x * yr0, y0, z * yr0 );
                vertPointer->position = vertPointer->normal * r;
                vertPointer++;
            }
        }
    }

    void Render( float scale = 1 );
};

struct Renderer {
    SphereRenderModel sphere = { 90 };

    Image textureEarth = { "..\\assets\\2k_earth_daymap.bmp" };
    Image textureStars = { "..\\assets\\2k_stars_milky_way.bmp" };

    int dlStars;
    int dlSphere;

    Renderer();
    ~Renderer();
    void Render(Simulation &simulation );

    void DrawStars();
    void DrawEarth();
};

