find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)

qt5_add_resources(qml qml/qml.qrc)

include_directories(${POPPLER_INCLUDES_DIR})

add_executable(harbour-pdf
  ${qml}
  main.cpp
  poppler-document.cpp
  document-view.cpp
  document-page.cpp
  tile-cache.cpp
  )
target_link_libraries(harbour-pdf -L${POPPLER_LIB_DIR} -lpoppler-qt5 -lpoppler -lpng12 -lz -lpthread -lfreetype -ltiff -ljpeg -lfontconfig)

qt5_use_modules(harbour-pdf Gui Quick Xml)
