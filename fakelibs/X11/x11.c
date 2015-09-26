#include "Xlib.h"

Display *XOpenDisplay(char *name) {}
Atom XInternAtom(Display *display, char *atom_name, Bool only_if_exists) {}
int DefaultScreen(Display *display) {}
Visual *DefaultVisual(Display *display, int screen_number) {}
Cursor XCreateFontCursor(Display *display, unsigned int shape) {}
Status XAllocColor(Display *display, Colormap colormap, XColor *screen_in_out) {}
Colormap DefaultColormap(Display *display, int screen_number) {}
Window DefaultRootWindow(Display *display) {}
int XSetWindowColormap(Display *display, Window w, Colormap colormap) {}
GC XCreateGC(Display *display, Drawable d, unsigned long valuemask, XGCValues *values) {}
int XDefineCursor(Display *display, Window w, Cursor cursor) {}
int XSelectInput(Display *display, Window w, long event_mask) {}
XWMHints *XAllocWMHints(void) {}
int XSetWMHints(Display *display, Window w, XWMHints *wmhints) {}
int XFree(void *data) {}
XClassHint *XAllocClassHint(void) {}
int XSetClassHint(Display *display, Window w, XClassHint *class_hints) {}
Status XSetWMProtocols(Display *display, Window w, Atom *protocols, int count) {}
int ConnectionNumber(Display *display) {}
int XFreeGC(Display *display, GC gc) {}
int XFreeCursor(Display *display, Cursor cursor) {}
int XChangeGC(Display *display, GC gc, unsigned long valuemask, XGCValues *values) {}
int XLookupString(XKeyEvent *event_struct, char *buffer_return, int bytes_buffer,
		  KeySym *keysym_return, XComposeStatus *status_in_out) {}
int DisplayHeight(Display *display, int screen_number) {}
int XMapWindow(Display *display, Window w) {}
int XDestroyWindow(Display *display, Window w) {}
int XFreePixmap(Display *display, Pixmap pixmap) {}
int XCloseDisplay(Display *display) {}
int DisplayWidth(Display *display, int screen_number) {}
int DisplayWidthMM(Display *display, int screen_number) {}
int XFlush(Display *display) {}
int XStoreName(Display *display, Window w, char *window_name) {}
int XNextEvent(Display *display, XEvent *event_return) {}
int XSetSelectionOwner(Display *display, Atom selection, Window owner, Time time) {}
int XSetForeground(Display *display, GC gc, unsigned long foreground) {}
unsigned long WhitePixel(Display *display, int screen_number) {}
unsigned long BlackPixel(Display *display, int screen_number) {}
int XPending(Display *display) {}
int XDrawString(Display *display, Drawable d, GC gc, int x, int y, char *string, int length) {}
Status XGetGCValues(Display *display, GC gc, unsigned long valuemask, XGCValues *values_return) {}
Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask,
		  XEvent *event_send) {}
int XConfigureWindow(Display *display, Window w, unsigned value_mask, XWindowChanges *changes) {}
int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format,
		    int mode, unsigned char *data, int nelements) {}
void XmbSetWMProperties(Display *display, Window w, char *window_name, char *icon_name,
			char *argv[], int argc, XSizeHints *normal_hints, XWMHints *wm_hints,
			XClassHint *class_hints) {}
int XFillRectangle(Display *display, Drawable d, GC gc, int x, int y, unsigned int width,
		   unsigned int height) {}
Window XCreateWindow(Display *display, Window window, int x, int y,
		     unsigned int width, unsigned int height, unsigned int border_width,
		     int depth, unsigned int class, Visual *visual,
		     unsigned long valuemask, XSetWindowAttributes *attributes) {}
Pixmap XCreateBitmapFromData(Display *display, Drawable d, char *data, unsigned int width,
			     unsigned int height) {}
int XSync(Display *display, Bool discard) {}
Bool XShmQueryExtension(Display *display) {}
XImage *XShmCreateImage (Display *display, Visual *visual, unsigned int depth,
			 int format, char *data, XShmSegmentInfo *shminfo,
			 unsigned int width, unsigned int height) {}
int XDestroyImage(XImage *ximage) {}
Bool XShmAttach(Display *display, XShmSegmentInfo *shminfo) {}
XImage *XCreateImage(Display *display, Visual *visual, unsigned int depth, int format,
		     int offset, char *data, unsigned int width, unsigned int height,
		     int bitmap_pad, int bytes_per_line) {}
Colormap XCreateColormap(Display *display, Window w, Visual *visual, int alloc) {}
Window RootWindow(Display *display, int screen_number) {}
int XStoreColors(Display *display, Colormap colormap, XColor color[], int ncolors) {}
int ImageByteOrder(Display *display) {}
VisualID XVisualIDFromVisual(Visual *visual) {}
XVisualInfo *XGetVisualInfo(Display *display, long vinfo_mask, XVisualInfo *vinfo_template,
			    int *nitems_return) {}
XPixmapFormatValues *XListPixmapFormats(Display *display, int *count_return) {}
Bool XQueryExtension(Display *display, const char *name, int *major_opcode_return,
		     int *first_event_return, int *first_error_return) {}
Bool XShmPutImage(Display *display, Drawable d, GC gc, XImage *image,
		  int src_x, int src_y, int dest_x, int dest_y,
		  unsigned int width, unsigned int height,
		  Bool send_event) {}
int XPutImage(Display *display, Drawable d, GC gc, XImage *image, int src_x, int src_y,
	      int dest_x, int dest_y, unsigned int width, unsigned int height) {}
int XGetErrorText(Display *display, int code, char *buffer_return, int length) {}

int (*XSetErrorHandler(int (*handler)(Display *, XErrorEvent *)))() {}
