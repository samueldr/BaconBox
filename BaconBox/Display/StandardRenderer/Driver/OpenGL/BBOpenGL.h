#ifndef BB_OPENGL_H
#define BB_OPENGL_H

#include "BaconBox/PlatformFlagger.h"

#if defined (BB_OPENGL) || defined (BB_OPENGLES)



#if defined(BB_SDL)
#if defined(BB_MAC_PLATFORM)
#include <OpenGL/OpenGL.h>
#elif defined(BB_WINDOWS_PLATFORM)
#define GLEW_STATIC
#include "GL/glew.h"
#define BB_GLEW
//#include <gl/gl.h>
//#include <gl/glu.h>
//#include "glext.h"
#elif defined(BB_LINUX)
#define GLEW_STATIC
#include <GL/glew.h>
#define BB_GLEW
#include <GL/glu.h>
#endif
#endif



#if defined(BB_IPHONE_PLATFORM)
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#elif defined(BB_QT)
#include <QtOpenGL>
#elif defined(BB_MAC_PLATFORM)
#include <OpenGL/gl.h>
#elif defined(BB_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#endif

#endif

#endif
