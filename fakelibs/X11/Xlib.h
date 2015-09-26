#ifndef XLIB_H
#define XLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#define Bool                     int
#define Status                   int
#define True                     1
#define False                    0
#define None                     0L

#define XC_left_ptr              68
#define XC_hand2                 60
#define XC_watch                 150
#define XC_xterm                 152
#define StructureNotifyMask      (1L<<17)
#define ExposureMask             (1L<<15)
#define KeyPressMask             (1L<<0)
#define ButtonReleaseMask        (1L<<3)
#define PointerMotionMask        (1L<<6)
#define ButtonPressMask          (1L<<2)
#define IconPixmapHint           (1L << 2)
#define IconMaskHint             (1L << 5)
#define MapNotify                19
#define PropModeReplace          0
#define InputOutput              1
#define ConfigureNotify          22
#define CWWidth                  (1<<2)
#define CWHeight                 (1<<3)
#define ClientMessage            33
#define SubstructureRedirectMask (1L<<20)
#define SubstructureNotifyMask   (1L<<19)
#define GCFunction               (1L<<0)
#define GXxor                    0x6
#define XA_PRIMARY               ((Atom) 1)
#define SelectionNotify          31
#define MotionNotify             6
#define ButtonPress              4
#define ControlMask              (1<<2)
#define XA_STRING                ((Atom) 31)
#define Expose                   12
#define KeyPress                 2
#define XK_Left                  0xff51
#define XK_Up                    0xff52
#define XK_Right                 0xff53
#define XK_Down                  0xff54
#define XK_Page_Up               0xff55
#define XK_Escape                0xff1b
#define XK_Page_Down             0xff56
#define XK_c                     0x0063
#define XF86XK_Back              0x1008FF26
#define XF86XK_Forward           0x1008FF27
#define SelectionRequest         30
#define ButtonRelease            5
#define ZPixmap                  2
#define AllocAll                 1
#define PseudoColor              3
#define DoRed                    (1<<0)
#define DoGreen                  (1<<1)
#define DoBlue                   (1<<2)
#define TrueColor                4
#define LSBFirst                 0
#define MSBFirst                 1
#define VisualIDMask             0x1

  typedef unsigned long int XID;
  typedef unsigned long int VisualID;
  typedef int Window;
  typedef int Drawable;
  typedef int Colormap;
  typedef int Atom;
  typedef int GC;
  typedef int Cursor;
  typedef int KeySym;
  typedef int Time;
  typedef int Pixmap;

  typedef struct {} Display;
  typedef struct {} Font;
  typedef struct {} Visual;
  typedef struct { int width, height; } XConfigureEvent;
  typedef struct { unsigned short red, green, blue; unsigned long pixel; char flags; } XColor;
  typedef struct { int flags, icon_mask; Pixmap icon_pixmap; } XWMHints;
  typedef struct { char *res_name, *res_class; } XClassHint;
  typedef struct {} XSetWindowAttributes;
  typedef struct { int function; } XGCValues;
  typedef struct {} XComposeStatus;
  typedef struct { unsigned int state; } XKeyEvent;
  typedef struct {} XSizeHints;
  typedef struct { int width, height; } XWindowChanges;
  typedef struct { int x, y; unsigned int state; } XMotionEvent;
  typedef struct { int x, y; unsigned int button; Time time; unsigned int state; } XButtonEvent;

  typedef struct {
    int class;
    int depth;
    Visual *visual;
    unsigned long red_mask;
    unsigned long green_mask;
    unsigned long blue_mask;
    VisualID visualid;
  } XVisualInfo;

  typedef struct {
    Window requestor;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
  } XSelectionRequestEvent;

  typedef struct {
    int type;
    Bool send_event;
    Window requestor;
    Display *display;
    Atom selection;
    Atom target;
    Atom property;
    Time time;
  } XSelectionEvent;

  typedef struct {
    int type;
    int serial;
    Bool send_event;
    Window window;
    Atom message_type;
    int format;

    union {
      char b[20];
      short s[10];
      long l[5];
    } data;
  } XClientMessageEvent;

  typedef union  {
    int type;
    XConfigureEvent xconfigure;
    XClientMessageEvent xclient;
    XSelectionEvent xselection;
    XSelectionRequestEvent xselectionrequest;
    XKeyEvent xkey;
    XMotionEvent xmotion;
    XButtonEvent xbutton;
  } XEvent;

  typedef struct {
    int width, height;
    int bytes_per_line;
    char *data;
  } XImage;

  typedef struct {
    int shmid;
    char *shmaddr;
    Bool readOnly;
  } XShmSegmentInfo;

  typedef struct { int depth, bits_per_pixel; } XPixmapFormatValues;
  typedef struct { unsigned char request_code, error_code; } XErrorEvent;

  int (*XSetErrorHandler(int (*handler)(Display *, XErrorEvent *)))();

  Display *XOpenDisplay(char *name);
  Atom XInternAtom(Display *display, char *atom_name, Bool only_if_exists);
  int DefaultScreen(Display *display);
  Visual *DefaultVisual(Display *display, int screen_number);
  Cursor XCreateFontCursor(Display *display, unsigned int shape);
  Status XAllocColor(Display *display, Colormap colormap, XColor *screen_in_out);
  Colormap DefaultColormap(Display *display, int screen_number);
  Window DefaultRootWindow(Display *display);
  int XSetWindowColormap(Display *display, Window w, Colormap colormap);
  GC XCreateGC(Display *display, Drawable d, unsigned long valuemask, XGCValues *values);
  int XDefineCursor(Display *display, Window w, Cursor cursor);
  int XSelectInput(Display *display, Window w, long event_mask);
  XWMHints *XAllocWMHints(void);
  int XSetWMHints(Display *display, Window w, XWMHints *wmhints);
  int XFree(void *data);
  XClassHint *XAllocClassHint(void);
  int XSetClassHint(Display *display, Window w, XClassHint *class_hints);
  Status XSetWMProtocols(Display *display, Window w, Atom *protocols, int count);
  int ConnectionNumber(Display *display);
  int XFreeGC(Display *display, GC gc);
  int XFreeCursor(Display *display, Cursor cursor);
  int XChangeGC(Display *display, GC gc, unsigned long valuemask, XGCValues *values);
  int XLookupString(XKeyEvent *event_struct, char *buffer_return, int bytes_buffer,
		    KeySym *keysym_return, XComposeStatus *status_in_out);
  int DisplayHeight(Display *display, int screen_number);
  int XMapWindow(Display *display, Window w);
  int XDestroyWindow(Display *display, Window w);
  int XFreePixmap(Display *display, Pixmap pixmap);
  int XCloseDisplay(Display *display);
  int DisplayWidth(Display *display, int screen_number);
  int DisplayWidthMM(Display *display, int screen_number);
  int XFlush(Display *display);
  int XStoreName(Display *display, Window w, char *window_name);
  int XNextEvent(Display *display, XEvent *event_return);
  int XSetSelectionOwner(Display *display, Atom selection, Window owner, Time time);
  int XSetForeground(Display *display, GC gc, unsigned long foreground);
  unsigned long WhitePixel(Display *display, int screen_number);
  unsigned long BlackPixel(Display *display, int screen_number);
  int XPending(Display *display);
  int XDrawString(Display *display, Drawable d, GC gc, int x, int y, char *string, int length);
  Status XGetGCValues(Display *display, GC gc, unsigned long valuemask, XGCValues *values_return);
  Status XSendEvent(Display *display, Window w, Bool propagate, long event_mask,
		    XEvent *event_send);
  int XConfigureWindow(Display *display, Window w, unsigned value_mask, XWindowChanges *changes);
  int XChangeProperty(Display *display, Window w, Atom property, Atom type, int format,
		      int mode, unsigned char *data, int nelements);
  void XmbSetWMProperties(Display *display, Window w, char *window_name, char *icon_name,
			  char *argv[], int argc, XSizeHints *normal_hints, XWMHints *wm_hints,
			  XClassHint *class_hints);
  int XFillRectangle(Display *display, Drawable d, GC gc, int x, int y, unsigned int width,
		     unsigned int height);
  Window XCreateWindow(Display *display, Window window, int x, int y,
		       unsigned int width, unsigned int height, unsigned int border_width,
		       int depth, unsigned int class, Visual *visual,
		       unsigned long valuemask, XSetWindowAttributes *attributes);
  Pixmap XCreateBitmapFromData(Display *display, Drawable d, char *data, unsigned int width,
			       unsigned int height);
  int XSync(Display *display, Bool discard);
  Bool XShmQueryExtension(Display *display);
  XImage *XShmCreateImage (Display *display, Visual *visual, unsigned int depth,
			   int format, char *data, XShmSegmentInfo *shminfo,
			   unsigned int width, unsigned int height);
  int XDestroyImage(XImage *ximage);
  Bool XShmAttach(Display *display, XShmSegmentInfo *shminfo);
  XImage *XCreateImage(Display *display, Visual *visual, unsigned int depth, int format,
		       int offset, char *data, unsigned int width, unsigned int height,
		       int bitmap_pad, int bytes_per_line);
  Colormap XCreateColormap(Display *display, Window w, Visual *visual, int alloc);
  Window RootWindow(Display *display, int screen_number);
  int XStoreColors(Display *display, Colormap colormap, XColor color[], int ncolors);
  int ImageByteOrder(Display *display);
  VisualID XVisualIDFromVisual(Visual *visual);
  XVisualInfo *XGetVisualInfo(Display *display, long vinfo_mask, XVisualInfo *vinfo_template,
			      int *nitems_return);
  XPixmapFormatValues *XListPixmapFormats(Display *display, int *count_return);
  Bool XQueryExtension(Display *display, const char *name, int *major_opcode_return,
		       int *first_event_return, int *first_error_return);
  Bool XShmPutImage(Display *display, Drawable d, GC gc, XImage *image,
		    int src_x, int src_y, int dest_x, int dest_y,
		    unsigned int width, unsigned int height,
		    Bool send_event);
  int XPutImage(Display *display, Drawable d, GC gc, XImage *image, int src_x, int src_y,
		int dest_x, int dest_y, unsigned int width, unsigned int height);
  int XGetErrorText(Display *display, int code, char *buffer_return, int length);

#ifdef __cplusplus
};
#endif

#endif /* XLIB_H */
