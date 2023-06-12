add_definitions(-D_KLOG_MODULE_NAME_=logi_applets_obs_plugin)
add_definitions(-DUNICODE)
add_definitions(-D_UNICODE)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG")

set(APPLET_NAME "logi_obs_plugin")

set(SOURCES
    src/obs_plugin.cpp)

set(HEADERS
    src/obs_plugin.hpp)

if(WIN32)
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8") 
        set(LIBS 
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/x64/obs.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/libs/x64/obs-frontend-api.lib)
    endif()

    if(COMMAND generate_product_version)
        generate_product_version(VERSION
        PRODUCT_NAME "Logitech G OBS Studio Plugin"
        PRODUCT_TYPE "VFT_DLL")
    endif()
    add_library(logi_applets_obs_plugin SHARED ${SOURCES} ${HEADERS} ${VERSION})
elseif(APPLE)
    set(LIBS 
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/osx/libobs.dylib
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/osx/obs-frontend-api.dylib)

    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-all-symbols")

    add_library(logi_applets_obs_plugin MODULE ${SOURCES} ${HEADERS} ${VERSION})
endif()


if(WIN32)
    # Rename the binary based on the bitness
    if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set_target_properties(logi_applets_obs_plugin PROPERTIES OUTPUT_NAME "${APPLET_NAME}_x64")
    endif()
elseif(APPLE)
    set_target_properties(logi_applets_obs_plugin PROPERTIES OUTPUT_NAME "${APPLET_NAME}")
endif()

get_filename_component(DEPS_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

target_include_directories(logi_applets_obs_plugin
    PUBLIC
    include)

target_include_directories(logi_applets_obs_plugin
    SYSTEM PUBLIC
    ${DEPS_DIR}/other/obs-studio
    ${DEPS_DIR}/other/websocketpp
    ${DEPS_DIR}/other/json/include
    ${DEPS_DIR}/other/asio/asio/include)

target_link_libraries(logi_applets_obs_plugin
    ${LIBS})

if(LGHUB_BACKEND_TARGET)
    configure_warning_suppression(logi_applets_obs_plugin PRIVATE)
endif()
