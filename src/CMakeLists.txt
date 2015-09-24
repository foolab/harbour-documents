find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(BOOSTER qdeclarative5-boostable REQUIRED)

set(CMAKE_INSTALL_RPATH "/usr/share/harbour-documents/lib/")
set(CMAKE_SKIP_BUILD_RPATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

qt5_add_resources(qml qml/qml.qrc)
qt5_add_resources(icons ../icons/icons.qrc)

add_definitions(${BOOSTER_CFLAGS})
include_directories(${CMAKE_BINARY_DIR}/deps/usr/include/
  ${CMAKE_SOURCE_DIR}/external/
  ${BOOSTER_INCLUDE_DIRS})

add_library(popplerbackend STATIC poppler-backend.cpp)
qt5_use_modules(popplerbackend Gui Quick Xml)
add_dependencies(popplerbackend poppler)

add_library(mupdfbackend STATIC mupdf-backend.cpp)
qt5_use_modules(mupdfbackend Gui Quick Xml)
add_dependencies(mupdfbackend mupdf)

add_library(lobackend STATIC lo-backend.cpp)
qt5_use_modules(lobackend Gui Quick Xml)

add_executable(harbour-documents
  ${qml}
  ${icons}
  main.cpp
  document.cpp
  document-view.cpp
  document-page.cpp
  tile-cache.cpp
  backend.cpp
  documents-browser.cpp
  icon-provider.cpp
  document-loader.cpp
  document-settings.cpp
  )

add_dependencies(harbour-documents popplerbackend)
qt5_use_modules(harbour-documents Gui Quick Xml)

target_link_libraries(harbour-documents
  -Wl,-whole-archive -L.
  popplerbackend
  mupdfbackend
  lobackend
  -Wl,-no-whole-archive
  ${ZLIB_LIBRARIES}
  ${PNG_LIBRARIES}
  ${BOOSTER_LDFLAGS}
 -L${CMAKE_BINARY_DIR}/deps/usr/lib/ -lpoppler-qt5 -lpoppler -lpthread -lmupdf -lopenjp2 -ljbig2dec -ljpeg -lfreetype -ldl)

install(TARGETS harbour-documents DESTINATION "/usr/bin/")
