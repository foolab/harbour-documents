find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(PNG REQUIRED)
find_package(Freetype REQUIRED)
find_package(TIFF REQUIRED)
find_package(JPEG REQUIRED)
#find_package(JPEG REQUIRED)

qt5_add_resources(qml qml/qml.qrc)

include_directories(${POPPLER_INCLUDES_DIR})

add_executable(harbour-pdf
  ${qml}
  main.cpp
  poppler-document.cpp
  document-view.cpp
  document-page.cpp
  tile-cache.cpp
  tile-request.cpp
  )
target_link_libraries(harbour-pdf
  ${PNG_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  ${TIFF_LIBRARIES}
  ${JPEG_LIBRARIES}
  -lfontconfig
 -L${POPPLER_LIB_DIR} -lpoppler-qt5 -lpoppler -lpthread)

add_dependencies(harbour-pdf poppler)
qt5_use_modules(harbour-pdf Gui Quick Xml)
