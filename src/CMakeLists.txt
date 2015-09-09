find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Freetype REQUIRED)
find_package(TIFF REQUIRED)
find_package(JPEG REQUIRED)
find_package(ZLIB REQUIRED)

qt5_add_resources(qml qml/qml.qrc)

include_directories(${CMAKE_BINARY_DIR}/deps/usr/include/)

add_library(popplerbackend STATIC poppler-backend.cpp)
qt5_use_modules(popplerbackend Gui Quick Xml)
add_dependencies(popplerbackend poppler)

add_executable(harbour-pdf
  ${qml}
  main.cpp
  document.cpp
  document-view.cpp
  document-page.cpp
  tile-cache.cpp
  tile-request.cpp
  backend.cpp
  )
target_link_libraries(harbour-pdf
  popplerbackend
  ${FREETYPE_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${ZLIB_LIBRARIES}
  -lfontconfig
 -L${CMAKE_BINARY_DIR}/deps/usr/lib/ -lpoppler-qt5 -lpoppler -lpthread -lpng16)

add_dependencies(harbour-pdf popplerbackend)
qt5_use_modules(harbour-pdf Gui Quick Xml)
