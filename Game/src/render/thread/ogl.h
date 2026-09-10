#ifndef _xOGL_H
#define _xOGL_H

#include <utilitieslib/utils/wininclude.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "GL/glew.h"
#include "GL/wglew.h"

#include <GL/gl.h>
#include <GL/glu.h>

extern int gl_clamp_val;

#define glDrawArrays glDrawArraysWrapper
void glDrawArraysWrapper(GLenum mode, GLint first, GLsizei count);
#define glDrawElements glDrawElementsWrapper
void glDrawElementsWrapper(GLenum a, GLsizei b, GLenum c, const GLvoid *d);
#define glBegin(gl_enum) glBeginWrapper(gl_enum)
void glBeginWrapper(GLenum a);

GLenum gldGetError(void);
#define glGetError DO_NOT_USE

#ifndef WCW_STATEMANAGER
#undef glActiveTextureARB
#define glActiveTextureARB DO_NOT_USE
#undef glBindTexture
#define glBindTexture DO_NOT_USE
#undef glClientActiveTextureARB
#define glClientActiveTextureARB DO_NOT_USE
#undef glBlendFunc
#define glBlendFunc DO_NOT_USE
#endif

// undefine some function variants we should not be using

#undef glCopyTexSubImage3D
#undef glDrawRangeElements
#undef glTexImage3D
#undef glTexSubImage3D

#undef glActiveTexture
#undef glClientActiveTexture
#undef glCompressedTexImage1D
#undef glCompressedTexImage2D
#undef glCompressedTexImage3D
#undef glCompressedTexSubImage1D
#undef glCompressedTexSubImage2D
#undef glCompressedTexSubImage3D
#undef glGetCompressedTexImage
#undef glLoadTransposeMatrixd
#undef glLoadTransposeMatrixf
#undef glMultTransposeMatrixd
#undef glMultTransposeMatrixf
#undef glMultiTexCoord1d
#undef glMultiTexCoord1dv
#undef glMultiTexCoord1f
#undef glMultiTexCoord1fv
#undef glMultiTexCoord1i
#undef glMultiTexCoord1iv
#undef glMultiTexCoord1s
#undef glMultiTexCoord1sv
#undef glMultiTexCoord2d
#undef glMultiTexCoord2dv
#undef glMultiTexCoord2f
#undef glMultiTexCoord2fv
#undef glMultiTexCoord2i
#undef glMultiTexCoord2iv
#undef glMultiTexCoord2s
#undef glMultiTexCoord2sv
#undef glMultiTexCoord3d
#undef glMultiTexCoord3dv
#undef glMultiTexCoord3f
#undef glMultiTexCoord3fv
#undef glMultiTexCoord3i
#undef glMultiTexCoord3iv
#undef glMultiTexCoord3s
#undef glMultiTexCoord3sv
#undef glMultiTexCoord4d
#undef glMultiTexCoord4dv
#undef glMultiTexCoord4f
#undef glMultiTexCoord4fv
#undef glMultiTexCoord4i
#undef glMultiTexCoord4iv
#undef glMultiTexCoord4s
#undef glMultiTexCoord4sv
#undef glSampleCoverage

#undef glBlendColor
#undef glBlendEquation
#undef glBlendFuncSeparate
#undef glFogCoordPointer
#undef glFogCoordd
#undef glFogCoorddv
#undef glFogCoordf
#undef glFogCoordfv
#undef glMultiDrawArrays
#undef glMultiDrawElements
#undef glPointParameterf
#undef glPointParameterfv
#undef glPointParameteri
#undef glPointParameteriv
#undef glSecondaryColor3b
#undef glSecondaryColor3bv
#undef glSecondaryColor3d
#undef glSecondaryColor3dv
#undef glSecondaryColor3f
#undef glSecondaryColor3fv
#undef glSecondaryColor3i
#undef glSecondaryColor3iv
#undef glSecondaryColor3s
#undef glSecondaryColor3sv
#undef glSecondaryColor3ub
#undef glSecondaryColor3ubv
#undef glSecondaryColor3ui
#undef glSecondaryColor3uiv
#undef glSecondaryColor3us
#undef glSecondaryColor3usv
#undef glSecondaryColorPointer
#undef glWindowPos2d
#undef glWindowPos2dv
#undef glWindowPos2f
#undef glWindowPos2fv
#undef glWindowPos2i
#undef glWindowPos2iv
#undef glWindowPos2s
#undef glWindowPos2sv
#undef glWindowPos3d
#undef glWindowPos3dv
#undef glWindowPos3f
#undef glWindowPos3fv
#undef glWindowPos3i
#undef glWindowPos3iv
#undef glWindowPos3s
#undef glWindowPos3sv

#undef glBeginQuery
#undef glBindBuffer
#undef glBufferData
#undef glBufferSubData
#undef glDeleteBuffers
#undef glDeleteQueries
#undef glEndQuery
#undef glGenBuffers
#undef glGenQueries
#undef glGetBufferParameteriv
#undef glGetBufferPointerv
#undef glGetBufferSubData
#undef glGetQueryObjectiv
#undef glGetQueryObjectuiv
#undef glGetQueryiv
#undef glIsBuffer
#undef glIsQuery
#undef glMapBuffer
#undef glUnmapBuffer

#undef glAttachShader
#undef glBindAttribLocation
#undef glBlendEquationSeparate
#undef glCompileShader
#undef glCreateProgram
#undef glCreateShader
#undef glDeleteProgram
#undef glDeleteShader
#undef glDetachShader
#undef glDisableVertexAttribArray
#undef glDrawBuffers
#undef glEnableVertexAttribArray
#undef glGetActiveAttrib
#undef glGetActiveUniform
#undef glGetAttachedShaders
#undef glGetAttribLocation
#undef glGetProgramInfoLog
#undef glGetProgramiv
#undef glGetShaderInfoLog
#undef glGetShaderSource
#undef glGetShaderiv
#undef glGetUniformLocation
#undef glGetUniformfv
#undef glGetUniformiv
#undef glGetVertexAttribPointerv
#undef glGetVertexAttribdv
#undef glGetVertexAttribfv
#undef glGetVertexAttribiv
#undef glIsProgram
#undef glIsShader
#undef glLinkProgram
#undef glShaderSource
#undef glStencilFuncSeparate
#undef glStencilMaskSeparate
#undef glStencilOpSeparate
#undef glUniform1f
#undef glUniform1fv
#undef glUniform1i
#undef glUniform1iv
#undef glUniform2f
#undef glUniform2fv
#undef glUniform2i
#undef glUniform2iv
#undef glUniform3f
#undef glUniform3fv
#undef glUniform3i
#undef glUniform3iv
#undef glUniform4f
#undef glUniform4fv
#undef glUniform4i
#undef glUniform4iv
#undef glUniformMatrix2fv
#undef glUniformMatrix3fv
#undef glUniformMatrix4fv
#undef glUseProgram
#undef glValidateProgram
#undef glVertexAttrib1d
#undef glVertexAttrib1dv
#undef glVertexAttrib1f
#undef glVertexAttrib1fv
#undef glVertexAttrib1s
#undef glVertexAttrib1sv
#undef glVertexAttrib2d
#undef glVertexAttrib2dv
#undef glVertexAttrib2f
#undef glVertexAttrib2fv
#undef glVertexAttrib2s
#undef glVertexAttrib2sv
#undef glVertexAttrib3d
#undef glVertexAttrib3dv
#undef glVertexAttrib3f
#undef glVertexAttrib3fv
#undef glVertexAttrib3s
#undef glVertexAttrib3sv
#undef glVertexAttrib4Nbv
#undef glVertexAttrib4Niv
#undef glVertexAttrib4Nsv
#undef glVertexAttrib4Nub
#undef glVertexAttrib4Nubv
#undef glVertexAttrib4Nuiv
#undef glVertexAttrib4Nusv
#undef glVertexAttrib4bv
#undef glVertexAttrib4d
#undef glVertexAttrib4dv
#undef glVertexAttrib4f
#undef glVertexAttrib4fv
#undef glVertexAttrib4iv
#undef glVertexAttrib4s
#undef glVertexAttrib4sv
#undef glVertexAttrib4ubv
#undef glVertexAttrib4uiv
#undef glVertexAttrib4usv
#undef glVertexAttribPointer

#undef glUniformMatrix2x3fv
#undef glUniformMatrix2x4fv
#undef glUniformMatrix3x2fv
#undef glUniformMatrix3x4fv
#undef glUniformMatrix4x2fv
#undef glUniformMatrix4x3fv

#undef glBeginConditionalRender
#undef glBeginTransformFeedback
#undef glBindBufferBase
#undef glBindBufferRange
#undef glBindFragDataLocation
#undef glClampColor
#undef glClearBufferfi
#undef glClearBufferfv
#undef glClearBufferiv
#undef glClearBufferuiv
#undef glColorMaski
#undef glDisablei
#undef glEnablei
#undef glEndConditionalRender
#undef glEndTransformFeedback
#undef glGetBooleani_v
#undef glGetFragDataLocation
#undef glGetIntegeri_v
#undef glGetStringi
#undef glGetTexParameterIiv
#undef glGetTexParameterIuiv
#undef glGetTransformFeedbackVarying
#undef glGetUniformuiv
#undef glGetVertexAttribIiv
#undef glGetVertexAttribIuiv
#undef glIsEnabledi
#undef glTexParameterIiv
#undef glTexParameterIuiv
#undef glTransformFeedbackVaryings
#undef glUniform1ui
#undef glUniform1uiv
#undef glUniform2ui
#undef glUniform2uiv
#undef glUniform3ui
#undef glUniform3uiv
#undef glUniform4ui
#undef glUniform4uiv
#undef glVertexAttribI1i
#undef glVertexAttribI1iv
#undef glVertexAttribI1ui
#undef glVertexAttribI1uiv
#undef glVertexAttribI2i
#undef glVertexAttribI2iv
#undef glVertexAttribI2ui
#undef glVertexAttribI2uiv
#undef glVertexAttribI3i
#undef glVertexAttribI3iv
#undef glVertexAttribI3ui
#undef glVertexAttribI3uiv
#undef glVertexAttribI4bv
#undef glVertexAttribI4i
#undef glVertexAttribI4iv
#undef glVertexAttribI4sv
#undef glVertexAttribI4ubv
#undef glVertexAttribI4ui
#undef glVertexAttribI4uiv
#undef glVertexAttribI4usv
#undef glVertexAttribIPointer

#endif
