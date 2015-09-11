include(ExternalProject)

ExternalProject_Add(libpng
  URL ${CMAKE_CURRENT_SOURCE_DIR}/libpng
  CONFIGURE_COMMAND cmake -DPNG_SHARED=OFF -DPNG_TESTS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/deps/usr/ <SOURCE_DIR>
  )

ExternalProject_Add(poppler
  URL ${CMAKE_CURRENT_SOURCE_DIR}/poppler
  CONFIGURE_COMMAND sed -ie "s/Qt5Widgets >= 5.0.0//" <SOURCE_DIR>/configure.ac && sed -ie "s/tests demos//" <SOURCE_DIR>/qt5/Makefile.am && <SOURCE_DIR>/autogen.sh --enable-static --disable-shared --prefix=${CMAKE_BINARY_DIR}/deps/usr/ --enable-xpdf-headers=no --enable-build-type=release --enable-fixedpoint --enable-cmyk=no --enable-libopenjpeg=none --enable-zlib --enable-libcurl=no --disable-cairo-output --disable-poppler-glib --enable-introspection=no --disable-poppler-qt4 --enable-poppler-qt5 --disable-utils --disable-poppler-cpp
  )

ExternalProject_Add(openjpeg
  URL ${CMAKE_CURRENT_SOURCE_DIR}/openjpeg
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/deps/usr/ <SOURCE_DIR>
  )

ExternalProject_Add(jbig2dec
  DEPENDS libpng
  URL ${CMAKE_CURRENT_SOURCE_DIR}/ghostpdl/jbig2dec
  BUILD_IN_SOURCE 1
  CONFIGURE_COMMAND PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig/ <SOURCE_DIR>/autogen.sh --enable-static --disable-shared --prefix=${CMAKE_BINARY_DIR}/deps/usr/  CFLAGS=-I${CMAKE_CURRENT_SOURCE_DIR}/libpng/\ -I${CMAKE_BINARY_DIR}/deps/usr/include/libpng16/ LDFLAGS=-L${CMAKE_BINARY_DIR}/deps/usr/lib/ LIBS=-lpng16
  )

ExternalProject_Add(mupdf
  DEPENDS openjpeg jbig2dec
  URL ${CMAKE_CURRENT_SOURCE_DIR}/mupdf
  CONFIGURE_COMMAND /bin/true
  BUILD_IN_SOURCE 1
  # TODO: touching is a hack to avoid needing x11. What can I do if mupdf has a sucky build system?
  BUILD_COMMAND PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig ${CMAKE_CURRENT_SOURCE_DIR}/build_mupdf.sh ${CMAKE_BINARY_DIR}/deps/usr/include/ && touch build/release/mudraw && touch build/release/mutool && touch build/release/mupdf-x11 && touch build/release/mujstest && touch build/release/mupdf-x11-curl
  INSTALL_COMMAND DESTDIR=${CMAKE_BINARY_DIR}/deps/ PKG_CONFIG_PATH=${CMAKE_BINARY_DIR}/deps/usr/lib/pkgconfig ${CMAKE_CURRENT_SOURCE_DIR}/build_mupdf.sh ${CMAKE_BINARY_DIR}/deps/usr/include/ install
  )
