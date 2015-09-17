find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Freetype REQUIRED)
find_package(TIFF REQUIRED)
find_package(JPEG REQUIRED)
find_package(ZLIB REQUIRED)

qt5_add_resources(qml qml/qml.qrc)
qt5_add_resources(icons ../icons/icons.qrc)

include_directories(${CMAKE_BINARY_DIR}/deps/usr/include/)

add_library(popplerbackend STATIC poppler-backend.cpp)
qt5_use_modules(popplerbackend Gui Quick Xml)
add_dependencies(popplerbackend poppler)

add_library(mupdfbackend STATIC mupdf-backend.cpp)
qt5_use_modules(mupdfbackend Gui Quick Xml)
add_dependencies(mupdfbackend mupdf)

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
  )
target_link_libraries(harbour-documents
  -Wl,-whole-archive -L.
  popplerbackend
  mupdfbackend
  -Wl,-no-whole-archive
  ${FREETYPE_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${JPEG_LIBRARIES}
  ${ZLIB_LIBRARIES}
  -lfontconfig
 -L${CMAKE_BINARY_DIR}/deps/usr/lib/ -lpoppler-qt5 -lpoppler -lpthread -lpng16 -lmupdf -lopenjp2 -ljbig2dec -lcrypto)

add_dependencies(harbour-documents popplerbackend)
qt5_use_modules(harbour-documents Gui Quick Xml)
