#pragma once

typedef std::map<const char*, float> UniformsMap;

struct Image : AbstractImage {
    using AbstractImage::AbstractImage;
 
    GLuint handle = 0;
    virtual void Bind();
};

struct Shader : AbstractImage {
    using AbstractImage::AbstractImage;

#if 0
    __declspec( noinline ) Shader( const std::string& fileName ): AbstractImage(fileName) {
        Load();
    }
#endif

    GLuint handle = 0;
    void Bind();
    void UpdateUniforms( UniformsMap& uniforms );

protected:
    std::string vertexShaderSource, fragmentShaderSource;

    void Compile();
    virtual void Load();
};

struct Images :AbstractImages {
protected:
    AbstractImage* GenImage( const std::string& fileName ) {
        std::string prefix( "..\\glsl\\" );
        if ( !fileName.compare( 0, prefix.size(), prefix ) ) 
            return new Shader( fileName );
        return new Image( fileName );
    }

    virtual void DisableTexturing() {
        glDisable( GL_TEXTURE_2D );
        glUseProgram( 0 );
    }
};