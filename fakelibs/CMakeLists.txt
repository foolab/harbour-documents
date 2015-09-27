include_directories(${CMAKE_SOURCE_DIR}/fakelibs/)

add_library(X11 SHARED X11/x11.c)
set_target_properties(X11 PROPERTIES SOVERSION 6)

#install(TARGETS X11 DESTINATION ${CMAKE_BINARY_DIR}/deps/usr/lib)

add_library(GL SHARED GL/gl.c)
set_target_properties(GL PROPERTIES SOVERSION 1)

#install(TARGETS GL DESTINATION ${CMAKE_BINARY_DIR}/deps/usr/lib)
