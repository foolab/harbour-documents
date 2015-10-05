include_directories(${CMAKE_SOURCE_DIR}/fakelibs/)

add_library(X11 SHARED X11/x11.c)
set_target_properties(X11 PROPERTIES SOVERSION 6)

add_library(GL SHARED GL/gl.c)
set_target_properties(GL PROPERTIES SOVERSION 1)
