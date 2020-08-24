#version 110

uniform sampler2D tex0;

varying vec3 worldPos;
varying vec3 viewPos;

float hash( float n ) {
    return fract( sin( n ) * 43758.5453123 );
}

float noise( vec3 x ) {
    vec3 f = fract( x );
    float n = dot( floor( x ), vec3( 1.0, 157.0, 113.0 ) );
    return mix( mix( mix( hash( n + 0.0 ), hash( n + 1.0 ), f.x ),
        mix( hash( n + 157.0 ), hash( n + 158.0 ), f.x ), f.y ),
        mix( mix( hash( n + 113.0 ), hash( n + 114.0 ), f.x ),
            mix( hash( n + 270.0 ), hash( n + 271.0 ), f.x ), f.y ), f.z );
}

void main()
{   
//    gl_FragColor = texture2D(tex0, gl_TexCoord[0].st);
    vec3 pos = worldPos;
    vec3 n = -normalize(pos);
    float accum = 0.;
    for(int i=0; i<9; i++) {
		//pos += n;    	
		//accum += noise(pos);	
    }
    gl_FragColor = vec4(0, 0, 0, 0);
    gl_FragColor.r = noise(pos);
}