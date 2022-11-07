

#if(WIN32)
#    string(REGEX REPLACE "\\\\" "/" OGRE_HOME "$ENV{OGRE_HOME}")
#elseif(UNIX)
#    if(EXISTS "/usr/local/lib/OGRE-2.2")
#      set(OGRE_HOME "/usr/local/lib/OGRE-2.2")
#    elseif(EXISTS "/usr/local/lib/x86_64-linux-gnu/OGRE-2.2")
#      set(OGRE_HOME "/usr/local/lib/x86_64-linux-gnu/OGRE-2.2")
#    elseif(EXISTS "/usr/lib/OGRE-2.2")
#      set(OGRE_HOME "/usr/lib/OGRE-2.2")
#    elseif(EXISTS "/usr/lib/x86_64-linux-gnu/OGRE-2.2")
#      set(OGRE_HOME "/usr/lib/x86_64-linux-gnu/OGRE-2.2")
#    else ()
#      message(SEND_ERROR "Failed to find module path.")
#    endif(EXISTS "/usr/local/lib/OGRE-2.2")

#    if(EXISTS "/usr/local/include/OGRE-2.2")
#      set(OGRE_INC_SEARCH_PATH "/usr/local/include/OGRE-2.2")
#    elseif(EXISTS "/usr/include/OGRE-2.2")
#      set(OGRE_INC_SEARCH_PATH "/usr/include/OGRE-2.2")
#    else ()
#      message(SEND_ERROR "Failed to find include path.")
#    endif(EXISTS "/usr/local/include/OGRE-2.2")

#endif(WIN32)

#list(APPEND CMAKE_MODULE_PATH "${OGRE_HOME}/cmake/")

find_package(tesseract_common REQUIRED)
find_package(tesseract_geometry REQUIRED)
find_package(Ogre2 REQUIRED COMPONENTS HlmsPbs HlmsUnlit)
find_package(Qt5 COMPONENTS Core Widgets OpenGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(FreeImage REQUIRED VERSION 3.9)

if (${OGRE_VERSION} VERSION_LESS "2.2" OR ${OGRE_VERSION} VERSION_GREATER "2.3")
    message(FATAL_ERROR "${PROJECT_NAME} requires Ogre 2.2")
endif()

foreach(X IN LISTS OGRE_HlmsPbs_INCLUDE_DIRS)
    list(APPEND OGRE_HlmsCommon_INCLUDE_DIRS ${X}/../Common)
endforeach()


# Load variable for clang tidy args, compiler options and cxx version
tesseract_variables()

qt5_wrap_cpp(
  RENDERING_OGRE2_headers_MOC
  include/tesseract_qt/rendering/ogre2/ogre2_manager.h
  include/tesseract_qt/rendering/ogre2/ogre2_viewport.h
  include/tesseract_qt/rendering/ogre2/ogre2_widget.h)

add_library(${PROJECT_NAME}_ogre2
    ${RENDERING_OGRE2_headers_MOC}
    src/common/shader_param.cpp
    src/common/shader_params.cpp
    src/common/color.cpp
    src/common/image.cpp
    src/ogre2_conversions.cpp
    src/ogre2_manager.cpp
    src/ogre2_material.cpp
    src/ogre2_renderman.cpp
    src/ogre2_viewport.cpp
    src/ogre2_widget.cpp
)
target_link_libraries(${PROJECT_NAME}_ogre2
    tesseract::tesseract_common
    tesseract::tesseract_geometry
    ${FreeImage_LIBRARIES}
    ${OGRE_LIBRARIES}
    ${OGRE_HlmsPbs_LIBRARIES}
    ${OGRE_HlmsUnlit_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    ${PROJECT_NAME}_common)
target_include_directories(
  ${PROJECT_NAME}_ogre2 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                        "$<INSTALL_INTERFACE:include>")
target_include_directories(${PROJECT_NAME}_ogre2 SYSTEM PUBLIC
    ${FreeImage_INCLUDE_DIRS}
    ${OGRE_INCLUDE_DIRS}
    ${OGRE_HlmsCommon_INCLUDE_DIRS}
    ${OGRE_HlmsPbs_INCLUDE_DIRS}
    ${OGRE_HlmsUnlit_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME}_ogre2 PUBLIC ${TESSERACT_COMPILE_DEFINITIONS}
                                                        TESSERACT_QT_OGRE2_RESOURCE_PATH="${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/ogre2")

#add_executable(${PROJECT_NAME}_ogre2_example src/basic_example.cpp)
#target_link_libraries(${PROJECT_NAME}_ogre2_example
#    ${OGRE_LIBRARIES}
#    ${OGRE_HlmsPbs_LIBRARIES}
#    ${OGRE_HlmsUnlit_LIBRARIES})
#target_include_directories(
#  ${PROJECT_NAME}_ogre2_example PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
#                                        "$<INSTALL_INTERFACE:include>")
#target_include_directories(${PROJECT_NAME}_ogre2_example SYSTEM PUBLIC
#    ${OGRE_INCLUDE_DIRS}
#    ${OGRE_HlmsCommon_INCLUDE_DIRS}
#    ${OGRE_HlmsPbs_INCLUDE_DIRS}
#    ${OGRE_HlmsUnlit_INCLUDE_DIRS})

#if(UNIX)
#    file(COPY ${OGRE_MEDIA_DIR}/../plugins.cfg DESTINATION ${CMAKE_BINARY_DIR})
#    file(COPY ${OGRE_MEDIA_DIR}/../plugins_d.cfg DESTINATION ${CMAKE_BINARY_DIR})

#    #install resources
#    file(COPY ${PROJECT_SOURCE_DIR}/common
#        DESTINATION ${CMAKE_BINARY_DIR}/../)

#	#install resources from /bin
#	file(COPY ${PROJECT_SOURCE_DIR}/bin/resources.cfg ${PROJECT_SOURCE_DIR}/bin/resources_d.cfg
#		${PROJECT_SOURCE_DIR}/bin/resources_default.cfg ${PROJECT_SOURCE_DIR}/bin/settings.cfg
#		${PROJECT_SOURCE_DIR}/bin/settings_default.cfg ${PROJECT_SOURCE_DIR}/bin/models.cfg
#		#QSS
#		${PROJECT_SOURCE_DIR}/bin/dark.qss
#		DESTINATION ${CMAKE_BINARY_DIR})
#endif()

add_subdirectory(demo)

install_targets(
  TARGETS
  ${PROJECT_NAME}_ogre2)
#  ${PROJECT_NAME}_ogre2_example)

install(DIRECTORY "media" DESTINATION share/${PROJECT_NAME}/ogre2/)
