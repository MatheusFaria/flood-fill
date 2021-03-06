# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (lab_project)

# CMakeLists modified from: http://www.opengl-tutorial.org/download/

find_package(OpenGL REQUIRED)


if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()



# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/sfml-cmake-modules")
include(FindSFML)
find_package(SFML COMPONENTS audio system)
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)

#Source: http://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  #-Wno-comment Will disable a commom warning when you are using GLM lib
  #-Wno-variadic-macros Is to permit the use of ... on the debug macros
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -W -Wall -Wno-long-long -Wno-comment -pedantic -Wno-variadic-macros -Wno-reorder")
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  SET(CMAKE_CXX_COMPILER "/usr/bin/clang++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-c++11-extensions")
ENDIF()


include_directories(
	external/glfw-3.0.3/include/
	external/glm/
	external/glew-1.9.0/include/
    external/freetype2/
        /usr/include/freetype2
        /usr/local/include/freetype2
	project/engine/headers/
	project/flood-fill/headers/

    ${SFML_INCLUDE_DIR}
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
    freetype
    ${SFML_LIBRARIES}
)

link_directories(
        /usr/local/lib/
)
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(
        main

        ### Flood Fill Files ###
        #Source Files
        project/flood-fill/source/main.cpp
        project/flood-fill/source/global_variables.cpp
        project/flood-fill/source/player.cpp
        project/flood-fill/source/ground_plane.cpp
        project/flood-fill/source/debug_player.cpp
        project/flood-fill/source/cinematic_player.cpp
        #project/flood-fill/source/demo_level.cpp
        project/flood-fill/source/box.cpp
        project/flood-fill/source/level_design.cpp
        project/flood-fill/source/border.cpp
        project/flood-fill/source/fluid_projectile.cpp
        project/flood-fill/source/fluid_box.cpp
        project/flood-fill/source/water_surface.cpp
        project/flood-fill/source/water_surface_manager.cpp
        project/flood-fill/source/level_template.cpp
        #project/flood-fill/source/temp_level.cpp
        project/flood-fill/source/test_level.cpp
        project/flood-fill/source/void_voxel.cpp
        project/flood-fill/source/solid_cube.cpp
        project/flood-fill/source/regular_polygons_render.cpp
        project/flood-fill/source/textured_polygons_render.cpp
        project/flood-fill/source/water_render.cpp
        project/flood-fill/source/normal_map_render.cpp
        project/flood-fill/source/player_hand.cpp
        project/flood-fill/source/water_stream_render.cpp
        project/flood-fill/source/water_particle_render.cpp
        project/flood-fill/source/camera_polygons_render.cpp
        project/flood-fill/source/debug_render.cpp
        project/flood-fill/source/action_invisible_block.cpp
        project/flood-fill/source/winning_block.cpp
        project/flood-fill/source/switch.cpp
        project/flood-fill/source/active_terrain.cpp
        project/flood-fill/source/color_change.cpp
        project/flood-fill/source/shadow_occluder_render.cpp
        project/flood-fill/source/normal_map_border_render.cpp
        project/flood-fill/source/tutorial_level.cpp
        project/flood-fill/source/tunnel_level.cpp
        project/flood-fill/source/boss_level.cpp
        project/flood-fill/source/text_render.cpp
        project/flood-fill/source/menu_item.cpp
        project/flood-fill/source/page.cpp
        project/flood-fill/source/menu.cpp
        project/flood-fill/source/switch_page_button.cpp
        project/flood-fill/source/level_button.cpp
        project/flood-fill/source/list_button.cpp
        project/flood-fill/source/billboard_text_render.cpp
        project/flood-fill/source/exclamation.cpp

        #Header Files
        #project/flood-fill/headers/main_level.hpp
        project/flood-fill/headers/global_variables.hpp
        project/flood-fill/headers/player.hpp
        project/flood-fill/headers/ground_plane.hpp
        project/flood-fill/headers/debug_player.hpp
        project/flood-fill/headers/cinematic_player.hpp
        #project/flood-fill/headers/demo_level.hpp
        project/flood-fill/headers/box.hpp
        project/flood-fill/headers/level_design.hpp
        project/flood-fill/headers/border.hpp
        project/flood-fill/headers/fluid_projectile.hpp
        project/flood-fill/headers/fluid_box.hpp
        project/flood-fill/headers/water_surface.hpp
        project/flood-fill/headers/water_surface_manager.hpp
        project/flood-fill/headers/level_template.hpp
        #project/flood-fill/headers/temp_level.hpp
        project/flood-fill/headers/test_level.hpp
        project/flood-fill/headers/void_voxel.hpp
        project/flood-fill/headers/solid_cube.hpp
        project/flood-fill/headers/regular_polygons_render.hpp
        project/flood-fill/headers/textured_polygons_render.hpp
        project/flood-fill/headers/water_render.hpp
        project/flood-fill/headers/normal_map_render.hpp
        project/flood-fill/headers/player_hand.hpp
        project/flood-fill/headers/water_stream_render.hpp
        project/flood-fill/headers/water_particle_render.hpp
        project/flood-fill/headers/camera_polygons_render.hpp
        project/flood-fill/headers/debug_render.hpp
        project/flood-fill/headers/action_invisible_block.hpp
        project/flood-fill/headers/winning_block.hpp
        project/flood-fill/headers/switch.hpp
        project/flood-fill/headers/active_terrain.hpp
        project/flood-fill/headers/color_change.hpp
        project/flood-fill/headers/shadow_occluder_render.hpp
        project/flood-fill/headers/normal_map_border_render.hpp
        project/flood-fill/headers/tutorial_level.hpp
        project/flood-fill/headers/boss_level.hpp
        project/flood-fill/headers/tunnel_level.hpp
        project/flood-fill/headers/text_render.hpp
        project/flood-fill/headers/menu_item.hpp
        project/flood-fill/headers/page.hpp
        project/flood-fill/headers/menu.hpp
        project/flood-fill/headers/switch_page_button.hpp
        project/flood-fill/headers/level_button.hpp
        project/flood-fill/headers/list_button.hpp
        project/flood-fill/headers/billboard_text_render.hpp
        project/flood-fill/headers/exclamation.hpp

        ### Engine Files ###
        #Source Files
        project/engine/source/tiny_obj_loader.cc
        project/engine/source/mesh.cpp
        project/engine/source/shader.cpp
        project/engine/source/object.cpp
        project/engine/source/camera.cpp
        project/engine/source/scene.cpp
        project/engine/source/load_manager.cpp
        project/engine/source/level_manager.cpp
        project/engine/source/director.cpp
        project/engine/source/game_object.cpp
        project/engine/source/time_manager.cpp
        project/engine/source/bounding_box.cpp
        project/engine/source/collision_manager.cpp
        project/engine/source/collision_object.cpp
        project/engine/source/light.cpp
        project/engine/source/material.cpp
        project/engine/source/render_engine.cpp
        project/engine/source/material_manager.cpp
        project/engine/source/image.cpp
        project/engine/source/texture.cpp
        project/engine/source/lodepng.cpp
        project/engine/source/render_element.cpp
        project/engine/source/sound.cpp
        project/engine/source/plane.cpp
        project/engine/source/render_grid_item.cpp
        project/engine/source/render_grid.cpp
        project/engine/source/render_texture.cpp
        project/engine/source/fbo.cpp
        project/engine/source/text_shader_utils.cpp
        project/engine/source/FontAtlas.cpp
        project/engine/source/FontEngine.cpp
        project/engine/source/text.cpp



        #Header Files
        project/engine/headers/GLIncludes.h
        project/engine/headers/tiny_obj_loader.h
        project/engine/headers/debug_macros.h
        project/engine/headers/mesh.hpp
        project/engine/headers/shader.hpp
        project/engine/headers/object.hpp
        project/engine/headers/camera.hpp
        project/engine/headers/scene.hpp
        project/engine/headers/load_manager.hpp
        project/engine/headers/level_manager.hpp
        project/engine/headers/director.hpp
        project/engine/headers/game_object.hpp
        project/engine/headers/time_manager.hpp
        project/engine/headers/bounding_box.hpp
        project/engine/headers/collision_manager.hpp
        project/engine/headers/collision_object.hpp
        project/engine/headers/light.hpp
        project/engine/headers/material.hpp
        project/engine/headers/render_engine.hpp
        project/engine/headers/material_manager.hpp
        project/engine/headers/uniform_3d_grid.hpp
        project/engine/headers/image.hpp
        project/engine/headers/texture.hpp
        project/engine/headers/lodepng.h
        project/engine/headers/sound.hpp
        project/engine/headers/render_element.hpp
        project/engine/headers/plane.hpp
        project/engine/headers/render_grid_item.hpp
        project/engine/headers/render_grid.hpp
        project/engine/headers/render_texture.hpp
        project/engine/headers/fbo.hpp
        project/engine/headers/text_shader_utils.h
        project/engine/headers/FontAtlas.h
        project/engine/headers/FontEngine.h
        project/engine/headers/text.hpp
)

target_link_libraries(main
	${ALL_LIBS}
)

# Xcode and Visual working directories
set_target_properties(main PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/project/")
create_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/")
create_default_target_launcher(main WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/")



SOURCE_GROUP(Shaders "project/flood-fill/shaders/.*glsl$" )


if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
#Copying new resources to the build folder
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/shaders" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/shaders"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/objects" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/objects"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/textures" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/textures"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/fonts" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/fonts"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/config-files" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/config-files"
)
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/project/flood-fill/sounds" "${CMAKE_CURRENT_BINARY_DIR}/flood-fill/sounds"
)


#Copying the compiled project to the project folder, out of the build directory
add_custom_command(
   TARGET main POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/main${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/project/"
)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )

