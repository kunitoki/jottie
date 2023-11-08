cmake_minimum_required(VERSION 3.27)

project(jottie_example VERSION 1.0.0)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG master
)
FetchContent_MakeAvailable(JUCE)

add_subdirectory(../ jottie)

juce_add_gui_app(jottie_example
    BUNDLE_ID                           org.kunitoki.jottie
    #ICON_BIG                            ${CMAKE_CURRENT_SOURCE_DIR}/Source/BinaryData/Icons/juce_icon.png
    DOCUMENT_EXTENSIONS                 lottie
    NEEDS_CURL                          FALSE
    NEEDS_WEB_BROWSER                   FALSE)

juce_generate_juce_header(jottie_example)

juce_add_binary_data(jottie_example_data SOURCES
    assets/cook.lottie)

target_sources(jottie_example PRIVATE
    sources/Main.cpp
    sources/JottieExample.h)

target_compile_definitions(jottie_example PRIVATE
    JUCE_ALLOW_STATIC_NULL_VARIABLES=0
    JUCE_LOG_ASSERTIONS=1
    JUCE_STRICT_REFCOUNTEDPOINTER=1
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(jottie_example PRIVATE
    jottie::jottie
    jottie_example_data
    juce::juce_build_tools
    juce::juce_gui_extra
    juce::juce_opengl
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
