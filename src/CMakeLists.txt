find_package(Qt5Gui REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

qt5_add_resources(qml qml/qml.qrc)

add_executable(harbour-pdf main.cpp ${qml})

qt5_use_modules(harbour-pdf Gui Quick)
