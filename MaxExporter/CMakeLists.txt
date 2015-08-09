cmake_minimum_required(VERSION 2.8)

project(MaxExporter)

set(SOURCE 
	AnimTrackMax.cpp
	AnimTrackMax.h
	CollisionTypes.h
	DllEntry.cpp
	exporterafx.h
	ExporterMax.cpp
	ExporterMax.h
	ExportModel.cpp
	ExportNode.cpp
	ExportTypes.h
	expstdafx.h
	FileExport.cpp
	MaterialExporter.cpp
	resource.h
	resourse.h
	ShapeObject.cpp
	SkeletonObjectMax.cpp
	SkeletonObjectMax.h
	UtilityForm.cpp
	UtilityForm.h
	NVMeshMender.cpp
	NVMeshMender.h
	)
	
add_library(MaxExporter STATIC ${SOURCE})