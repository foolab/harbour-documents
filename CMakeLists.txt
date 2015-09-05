cmake_minimum_required(VERSION 2.8.2)
project(HarbourPDF)

set(CMAKE_AUTOMOC ON)

include(ExternalProject)

ExternalProject_Add(poppler
  URL ${CMAKE_CURRENT_SOURCE_DIR}/poppler
  CONFIGURE_COMMAND <SOURCE_DIR>/autogen.sh --enable-static --disable-shared --prefix=<INSTALL_DIR> --enable-xpdf-headers=no --enable-build-type=release --enable-fixedpoint --enable-cmyk=no --enable-libopenjpeg=none --enable-zlib --enable-libcurl=no --disable-cairo-output --disable-poppler-glib --enable-introspection=no --disable-poppler-qt4 --enable-poppler-qt5 --disable-utils --disable-poppler-cpp
  )

ExternalProject_Get_Property(poppler install_dir)
set(POPPLER_INCLUDES_DIR ${install_dir}/include)
set(POPPLER_LIB_DIR ${install_dir}/lib)

add_subdirectory(src)
