cmake_minimum_required(VERSION 3.10)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

#find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_library(qribbon
	QRibbon.cpp
	QRibbon.h
	qribbon.ui
	QRibbon.qrc 
)

target_link_libraries(qribbon PRIVATE Qt6::Widgets)

add_library(QRibbon::qribbon ALIAS qribbon)
