#include "stdafx.h"

namespace GL20 {

    void Image::Bind() {
        if ( !handle ) {
            glGenTextures( 1, &handle );
            //printf( "Load texHandle %d\n", handle );
            glBindTexture( GL_TEXTURE_2D, handle );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
            glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
            glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
            auto const format = hasAlpha ? GL_RGBA : GL_RGB;
            glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data.data() );
        }
        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, handle );
        RendererParams::PC.textureSwitches++;
    }

    void Shader::Bind() {
        if ( !handle ) {
            Compile();
        }
        glUseProgram( handle );
        RendererParams::PC.shaderSwitches++;
    }

    void Shader::Unbind() {
        glUseProgram( 0 );
        if ( RendererParams::reloadShaders ) {
            glDeleteProgram( handle );
            handle = 0;
            state = State::Empty;
        }
    }

    void Shader::UpdateUniforms( UniformsMap& uniforms ) {
        abstractImages->Bind( this );
        if ( !handle )
            return;
        auto aspectRatio = glGetUniformLocation( handle, "aspectRatio" );
        glUniform1f( aspectRatio, RendererParams::aspectRatio );
        if ( uniforms.count( "time" ) ) {
            auto time = glGetUniformLocation( handle, "time" );
            glUniform1fv( time, 1, (const GLfloat*) uniforms["time"] );
        }
        if ( uniforms.count( "modelMatrix" ) ) {
            auto modelMatrix = glGetUniformLocation( handle, "modelMatrix" );
            glUniformMatrix4fv( modelMatrix, 1, GL_FALSE, (const GLfloat*) uniforms["modelMatrix"] );
        }
    }

    void glAssert( unsigned int obj, GLenum statusType, void ( APIENTRY* ivFun )( GLuint, GLenum, GLint* ),
        void ( APIENTRY* infoLogFun )( GLuint, GLsizei, GLsizei*, GLchar* ) ) {
        GLint statusCode = GL_FALSE;
        ivFun( obj, statusType, &statusCode );
        if ( statusCode == GL_TRUE ) {
            return;
        }

        GLint length = 0;
        ivFun( obj, GL_INFO_LOG_LENGTH, &length );

        char* error_log = (char*) alloca( length );
        infoLogFun( obj, length, &length, &error_log[0] );

        printf( "%s\n", error_log );
        //free( error_log );
        //exit( 0 );
    }

    unsigned int makeShader( const char* code, GLenum shaderType ) {
        unsigned int shader = glCreateShader( shaderType );
        glShaderSource( shader, 1, &code, NULL );
        glCompileShader( shader );

        glAssert( shader, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog );

        return shader;
    }

    void Shader::Compile() {
        unsigned int program = glCreateProgram();
        printf( "Compile shader %d\n", program );

        unsigned int vertexShader = !vertexShaderSource.empty() ? makeShader( vertexShaderSource.c_str(), GL_VERTEX_SHADER ) : 0;
        unsigned int fragmentShader = !fragmentShaderSource.empty() ? makeShader( fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER ) : 0;
        if ( vertexShader ) { glAttachShader( program, vertexShader ); }
        if ( fragmentShader ) { glAttachShader( program, fragmentShader ); }
        glLinkProgram( program );

        glAssert( program, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog );

        if ( vertexShader ) { glDetachShader( program, vertexShader ); }
        if ( vertexShader ) { glDeleteShader( vertexShader ); }
        if ( fragmentShader ) { glDetachShader( program, fragmentShader ); }
        if ( fragmentShader ) { glDeleteShader( fragmentShader ); }

        handle = program;
    }

    void Shader::Load() {
        hasAlpha = true;
        {
            std::ifstream t( fileName + ".vs" );
            std::string str( ( std::istreambuf_iterator<char>( t ) ),
                std::istreambuf_iterator<char>() );
            vertexShaderSource = str.c_str();
        }
        {
            std::ifstream t( fileName + ".fs" );
            std::string str( ( std::istreambuf_iterator<char>( t ) ),
                std::istreambuf_iterator<char>() );
            fragmentShaderSource = str.c_str();
        }
        state = AbstractImage::State::Loaded;
    }

}