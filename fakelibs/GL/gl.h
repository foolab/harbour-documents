#ifndef GL_H
#define GL_H

#include <X11/Xlib.h>

typedef unsigned int    GLenum;
typedef unsigned char   GLboolean;
typedef unsigned int    GLbitfield;
typedef void            GLvoid;
typedef int             GLint;          /* 4-byte signed */
typedef unsigned int    GLuint;         /* 4-byte unsigned */
typedef int             GLsizei;        /* 4-byte signed */
typedef float           GLfloat;        /* single precision float */
typedef unsigned char   GLubyte;        /* 1-byte unsigned */
typedef double          GLdouble;       /* double precision float */
typedef float           GLclampf;       /* single precision float in [0,1] */
typedef double          GLclampd;       /* double precision float in [0,1] */

void glFlush(void);
void glEnable(GLenum  cap);
void glDisable(GLenum  cap);
void glDrawArrays(GLenum  mode, GLint  first, GLsizei  count);
void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void glStencilMask( GLuint mask);
void glStencilFunc(GLenum func, GLint ref, GLuint mask);
void glStencilOp(GLenum  sfail, GLenum  dpfail, GLenum  dppass);
void glClear(GLbitfield  mask);
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
void glReadPixels(GLint  x, GLint  y, GLsizei  width, GLsizei  height, GLenum  format,
		  GLenum  type, GLvoid *  data);
void glBlendFunc(GLenum sfactor, GLenum dfactor);
void glGenTextures(GLsizei n, GLuint * textures);
void glBindTexture( GLenum target, GLuint texture);
void glTexParameteri(GLenum target, GLenum pname, GLint param);
void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
void glTexImage1D(GLenum target, GLint level, GLint internalFormat, GLsizei width,
		  GLint border,	GLenum format, GLenum type, const GLvoid * data);
void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width,
		  GLsizei height, GLint border, GLenum format, GLenum type,
		  const GLvoid * data);
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y,
		      GLsizei width, GLsizei height, GLint border);
void glPixelStorei(GLenum pname, GLint param);
void glDeleteTextures(GLsizei n, const GLuint * textures);
void glGetTexImage(GLenum  target, GLint  level, GLenum  format, GLenum  type, GLvoid *  img);
GLenum glGetError(void);
const GLubyte* glGetString(GLenum name);
void glPushMatrix(void);
void glPopMatrix(void);
void glTranslated(GLdouble  x, GLdouble  y, GLdouble  z);
void glMultMatrixd( const GLdouble *m);
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void glBegin(GLenum  mode);
void glEnd( void);
void glVertex2d(GLdouble x, GLdouble y);
void glTexCoord2f(GLfloat  s, GLfloat  t);
void glMatrixMode(GLenum mode);
void glLoadIdentity(void);
void glColor4d(GLdouble  red, GLdouble  green, GLdouble  blue, GLdouble  alpha);
void glHint(GLenum target, GLenum mode);
void glShadeModel(GLenum  mode);
void glScaled(GLdouble  x, GLdouble  y, GLdouble  z);
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
void glLoadMatrixd(const GLdouble * m);
void glGetIntegerv(GLenum  pname, GLint *  params);
void glCullFace(GLenum mode);
void glPolygonMode(GLenum face, GLenum mode);
void glDepthFunc(GLenum func);
void glLineWidth(GLfloat width);
void glClearDepth(GLclampd  depth);
void glDepthMask(GLboolean flag);
void glDrawElements( GLenum mode, GLsizei count, GLenum type, const GLvoid * indices);
void glPolygonOffset(GLfloat factor, GLfloat units);
void glFrontFace(GLenum  mode);
void glPointSize(GLfloat size);
void glMaterialfv(GLenum  face, GLenum  pname, const GLfloat *params);
void glLightfv(GLenum light, GLenum pname, const GLfloat * params);

/* X11 */
typedef void (*GLfunction)();

Bool glXQueryVersion(Display * dpy, int * major, int * minor);
const char *glXGetClientString(Display  *dpy, int name);
extern GLfunction glXGetProcAddressARB(const GLubyte *procName);

#endif /* GL_H */
