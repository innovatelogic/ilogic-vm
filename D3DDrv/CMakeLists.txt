cmake_minimum_required(VERSION 2.8)

project(D3DDriver)

add_definitions(-DUNICODE -D_UNICODE)

set(GLEW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
SET(GLEW_LIBRARY ${CMAKE_BINARY_DIR}/../Libs)

SET(GLEW_LIBRARIES optimized ${CMAKE_BINARY_DIR}/../Libs/glew32.lib debug ${CMAKE_BINARY_DIR}/../Libs/glew32d.lib)
SET(GLEW_LIBRARIES ${GLEW_LIBRARIES} optimized ${CMAKE_BINARY_DIR}/../Libs/glew32mx.lib debug ${CMAKE_BINARY_DIR}/../Libs/glew32mxd.lib)
SET(GLEW_LIBRARIES ${GLEW_LIBRARIES} optimized ${CMAKE_BINARY_DIR}/../Libs/libglew32d.lib debug ${CMAKE_BINARY_DIR}/../Libs/libglew32d.lib)
SET(GLEW_LIBRARIES ${GLEW_LIBRARIES} optimized ${CMAKE_BINARY_DIR}/../Libs/libglew32mxd.lib debug ${CMAKE_BINARY_DIR}/../Libs/libglew32mxd.lib)

#set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/../Libs)
#set(CMAKE_LIBRARY_PATH ${CMAKE_BINARY_DIR}/../Libs)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    #link_libraries(${GLEW_LIBRARIES})
#endif()

include_directories(${COMMON_SDK_DIR}/FreeType-2.1.4-lib/include)
include_directories(${COMMON_SDK_DIR}/FreeType-2.1.4-lib/include/freetype2)

#why its not done by GLEW find_package?
#link_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.9.0/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/FreeType-2.1.4-lib/lib)

set(LIBRARIES libfreetype) #glew32

set(SOURCE
	AnimFrame.cpp
	AnimFrame.h
	AnimSequence.cpp
	AnimSequence.h
	BaseMaterial.cpp
	BaseMaterial.h
	D3DAnimMesh.cpp
	D3DAnimMesh.h
	D3DDriver.cpp
	D3DDriver.h
	D3DDriver_Debug.cpp
	D3DDriver_Loader.cpp
	d3ddriverstdafx.h
	D3DFontMesh.cpp
	D3DFontMesh.h
	D3DMesh.cpp
	D3DMesh.h
	D3DRenderInterface.cpp
	D3DRenderInterface.h
	D3DRenderTarget.cpp
	D3DRenderTarget.h
	D3DShader.cpp
	D3DShader.h
	D3DSkeleton.cpp
	D3DSkeleton.h
	D3DSkeletonData.h
	D3DUtil.cpp
	D3DUtil.h
	DDSLoader.cpp
	dxstdafx.h
	Font2D.cpp
	Font2D.h
	FontNode.cpp
	FontNode.h
	MaterialEffect.cpp
	MaterialEffect.h
	MaterialEffect_ButtonStates.cpp
	MaterialEffect_ButtonStates.h
	MaterialEffectFont.cpp
	MaterialEffectFont.h
	MaterialEffectLava.cpp
	MaterialEffectLava.h
	MaterialEffectNode.cpp
	MaterialEffectNode.h
	MaterialEffectUI.cpp
	MaterialEffectUI.h
	NvTriStrip.h
	project.h
	Refcount.cpp
	Refcount.h
	RenderContext.cpp
	RenderContext.h
	RenderTargetNode.cpp
	RenderTargetNode.h
	SkeletonNode.cpp
	SkeletonNode.h
	SubMeshNode.cpp
	SubMeshNode.h
	Texture2D.cpp
	Texture2D.h
	Texture3D.cpp
	Texture3D.h
	TextureNode.cpp
	TextureNode.h
	VolumeTextureNode.cpp
	VolumeTextureNode.h)
	
	add_library(${PROJECT_NAME} SHARED ${SOURCE})
	
	target_link_libraries( ${PROJECT_NAME} ${LIBRARIES} ${GLEW_LIBRARIES})
	
	set_target_properties( ${PROJECT_NAME} PROPERTIES
                        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                        )
	
#install path
set(INSTALL_TARGETS "${PROJECT_SOURCE_DIR}/../common/SDK/FreeType-2.1.4-lib/bin/FreeType-6.dll") #"${PROJECT_SOURCE_DIR}/../common/SDK/glew-1.9.0/bin/glew32.dll"

#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
#COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
#        "${PROJECT_SOURCE_DIR}/../common/SDK/glew-1.9.0/bin/glew32.dll"      # <--this is in-file
#       $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path
		
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/../common/SDK/FreeType-2.1.4-lib/bin/FreeType-6.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)                 # <--this is out-file path

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})
	include_directories(${CMAKE_SOURCE_DIR}/utils/gtest-1.7.0/include)
	add_executable( ut.${PROJECT_NAME} 
					ut/ut_utils.cpp
					ut/main.cpp )
	
	target_link_libraries(ut.${PROJECT_NAME} ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

	set_target_properties(ut.${PROJECT_NAME} PROPERTIES FOLDER "UT" )

	set_target_properties( ut.${PROJECT_NAME} PROPERTIES
						RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
						RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
						)
endif( ${ENABLE_TESTING} )					