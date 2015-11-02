cmake_minimum_required(VERSION 2.8)

project(RenderSDK)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/common/SDK)
include_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/include)
include_directories(${CMAKE_SOURCE_DIR}/D3DDrv)
include_directories(${CMAKE_SOURCE_DIR}/TinyXML)
include_directories(${CMAKE_SOURCE_DIR}/Foundation)

#link_directories(${CMAKE_SOURCE_DIR}/common/SDK/glew-1.13.0/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/FreeType-2.1.4-lib/lib)
link_directories(${CMAKE_SOURCE_DIR}/common/SDK/nv_math/lib)

set(LIBRARIES Foundation D3DDriver TinyXML nv_math)

set(SOURCE 
	Allocator.h
	BoneObject.cpp
	BoneObject.h
	Comp_AnimMesh.cpp
	Comp_AnimMesh.h
	Comp_AnimTrack.cpp
	Comp_AnimTrack.h
	Comp_EnvSceneInfo.cpp
	Comp_EnvSceneInfo.h
	Comp_MaterialBaseEffect.cpp
	Comp_MaterialBaseEffect.h
	Comp_MaterialButton.cpp
	Comp_MaterialButton.h
	Comp_MaterialEffect.cpp
	Comp_MaterialEffect.h
	Comp_MaterialEffectFont.cpp
	Comp_MaterialEffectFont.h
	Comp_MaterialEffectLava.cpp
	Comp_MaterialEffectLava.h
	Comp_MaterialEffectUI.cpp
	Comp_MaterialEffectUI.h
	Comp_MeshNode.cpp
	Comp_MeshNode.h
	Comp_RenderTarget.cpp
	Comp_RenderTarget.h
	Comp_SkeletonObject.cpp
	Comp_SkeletonObject.h
	Comp_StaticMesh.cpp
	Comp_StaticMesh.h
	D3DMeshAllocator.cpp
	D3DMeshAllocator.h
	FXMatManager.cpp
	FXMatManager.h
	MaterialInstance.h
	m_animation.cpp
	m_animation.h
	m_book_set.h
	m_camera.cpp
	m_camera.h
	m_collision.h
	m_drawmodel_visitor.cpp
	m_drawmodel_visitor.h
	m_factory.cpp
	m_factory.h
	m_interface.cpp
	m_interface.h
	m_light.cpp
	m_light.h
	m_material.cpp
	m_material.h
	m_mesh.cpp
	m_mesh.h
	m_model.cpp
	m_model.h
	m_node.cpp
	m_node.h
	m_refcount.cpp
	m_refcount.h
	m_scene.cpp
	m_scene.h
	m_stdafx.h
	m_texcoord_set.cpp
	m_texcoord_set.h
	m_texture.cpp
	m_texture.h
	m_visitor.cpp
	m_visitor.h
	RenderFont.h
	RenderObject.cpp
	RenderObject.h
	RenderSDK.cpp
	RenderSDK.h
	RenderSDKInterface.cpp
	RenderSDKInterface.h
	RenderTypes.h
	StdafxRender.h
	UIComp_DrawFont.cpp
	UIComp_DrawFont.h
	UIComp_DrawFontState.cpp
	UIComp_DrawFontState.h
	UIComp_DrawImage.cpp
	UIComp_DrawImage.h
	UIComp_DrawImageState.cpp
	UIComp_DrawImageState.h
	UIComp_DrawMesh.cpp
	UIComp_DrawMesh.h
	URenderCamera.h
	VBObject.cpp
	VBObject.h
	VisualStyle.cpp
	VisualStyle.h
	RenderAdjacency.cpp
	RenderAdjacency.h
	variant_render_adj.h
	variant_render_command.h
	)
	
add_library(${PROJECT_NAME} SHARED ${SOURCE})

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)

#-------------------------------------------------------------------------------		
# setup testing
#-------------------------------------------------------------------------------
if (${ENABLE_TESTING})
	include_directories(${CMAKE_SOURCE_DIR}/utils/gtest-1.7.0/include)
	add_executable( ut.${PROJECT_NAME} 
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