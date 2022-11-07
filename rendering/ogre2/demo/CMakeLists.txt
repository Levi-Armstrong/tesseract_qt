find_package(tesseract_support REQUIRED)
find_package(tesseract_environment REQUIRED)

qt5_wrap_cpp(
  RENDERING_OGRE2_DEMO_headers_MOC
  ogre2_custom_widget.h)

add_library(${PROJECT_NAME}_ogre2_custom_widget ${RENDERING_OGRE2_DEMO_headers_MOC} ogre2_custom_widget.cpp)
target_link_libraries(${PROJECT_NAME}_ogre2_custom_widget
    ${OGRE_LIBRARIES}
    ${OGRE_HlmsPbs_LIBRARIES}
    ${OGRE_HlmsUnlit_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    ${PROJECT_NAME}_common
    ${PROJECT_NAME}_ogre2)
target_include_directories(
  ${PROJECT_NAME}_ogre2_custom_widget PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                        "$<INSTALL_INTERFACE:include>")
target_include_directories(${PROJECT_NAME}_ogre2_custom_widget SYSTEM PUBLIC
    ${OGRE_INCLUDE_DIRS}
    ${OGRE_HlmsCommon_INCLUDE_DIRS}
    ${OGRE_HlmsPbs_INCLUDE_DIRS}
    ${OGRE_HlmsUnlit_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}_ogre2_demo ogre2_demo.cpp)
target_link_libraries(${PROJECT_NAME}_ogre2_demo PRIVATE ${PROJECT_NAME}_ogre2_custom_widget tesseract::tesseract_support tesseract::tesseract_environment)
target_include_directories(${PROJECT_NAME}_ogre2_demo PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                             "$<INSTALL_INTERFACE:include>")

add_executable(${PROJECT_NAME}_ogre2_demo2 ogre2_demo2.cpp)
target_link_libraries(${PROJECT_NAME}_ogre2_demo2 PRIVATE ${PROJECT_NAME}_ogre2)
target_include_directories(${PROJECT_NAME}_ogre2_demo2 PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                          "$<INSTALL_INTERFACE:include>")


install_targets(
  TARGETS
  ${PROJECT_NAME}_ogre2_custom_widget
  ${PROJECT_NAME}_ogre2_demo
  ${PROJECT_NAME}_ogre2_demo2
)
