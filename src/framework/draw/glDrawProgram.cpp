#include "glDrawProgram.hpp"

#include <string.h>

GLDrawProgram *GLDrawProgram::bound = 0;

GLDrawProgram::GLDrawProgram() : destination(0)
{
    dirty = true;
    isAccum = false;
    id = glCreateProgram();
    memset(shaders, 0, sizeof(GLDrawShader*[5]));
}


GLDrawProgram::~GLDrawProgram()
{

}

GLDrawProgram *GLDrawProgram::fromVFShaderStrings(const char *vstring, const char *fstring)
{
    GLDrawShader *vshader = GLDrawShader::fromString(GLDrawShader::VERTEX_SHADER, vstring);
    GLDrawShader *fshader = GLDrawShader::fromString(GLDrawShader::FRAGMENT_SHADER, fstring);
    GLDrawProgram *program = new GLDrawProgram;
    program->bindStage(0, vshader);
    program->bindStage(1, fshader);
    return program;
}

void GLDrawProgram::clean()
{
    for(int i = 0; i < 5; i++)
    {
        if(shaders[i])
        {
            glAttachShader(id, shaders[i]->id);
        }
    }

    glLinkProgram(id);
}

static const unsigned glBuffers[] =
{
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
};

void GLDrawProgram::setDestination(GLFramebuffer *fb)
{
    destination = fb; 
}

void GLDrawProgram::bindTexture(const char *name, unsigned unit, GLTexture *tex)
{
    unsigned tid = 0;
    if(tex) tid = tex->id;

    glUseProgram(id);
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tid);
    glUniform1i(glGetUniformLocation(id, name), unit);
}

void GLDrawProgram::bindStage(int stage, GLDrawShader *program)
{
    if(shaders[stage])
    {
        glDetachShader(id, shaders[stage]->id);
    }

    shaders[stage] = program;
}

void GLDrawProgram::use()
{
    if(bound == this) return;

    if(dirty)
    {
        clean();
        dirty = false;
    }

    glUseProgram(id);

    if(isAccum)
    {
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    } else
    {
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    if(destination) destination->bind();
    else 
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
    }

    bound = this;
}

void GLDrawProgram::drawMesh(GLMesh *mesh)
{
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibuffer);
    GLuint pos_uint = glGetAttribLocation(id, "position");
    GLuint norm_uint = glGetAttribLocation(id, "normal");
    GLuint uv_uint = glGetAttribLocation(id, "uv");
    glEnableVertexAttribArray(pos_uint);
    glEnableVertexAttribArray(norm_uint);
    glEnableVertexAttribArray(uv_uint);
    glVertexAttribPointer(pos_uint, 3, GL_FLOAT, GL_FALSE, 24, 0);
    glVertexAttribPointer(norm_uint, 3, GL_SHORT, GL_TRUE, 24, (void*) 12);
    glVertexAttribPointer(uv_uint, 2, GL_UNSIGNED_SHORT, GL_TRUE, 24, (void*) 18);

    glDrawElements(GL_TRIANGLES, mesh->getNElements(), GL_UNSIGNED_SHORT, 0);
}

void GLDrawProgram::setUniform(const char *nm, int val)
{
    use();
    glUniform1i(glGetUniformLocation(id, nm), val);
}

void GLDrawProgram::setUniform(const char *nm, mat4 &matrix)
{
    use();
    glUniformMatrix4fv(glGetUniformLocation(id, nm), 1, GL_FALSE, matrix.ptr());
}

void GLDrawProgram::setUniform(const char *nm, vec4 vector)
{
    use();
    glUniform4fv(glGetUniformLocation(id, nm), 1, vector.ptr());
}
