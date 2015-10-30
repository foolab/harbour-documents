cmake_minimum_required(VERSION 2.8.2)
project(HarbourDocuments)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "-std=gnu++11")

add_subdirectory(fakelibs)
add_subdirectory(external)
add_subdirectory(src)

install(FILES harbour-documents.png DESTINATION "/usr/share/icons/hicolor/86x86/apps/")
install(FILES harbour-documents.desktop DESTINATION "/usr/share/applications/")
