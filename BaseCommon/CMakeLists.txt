cmake_minimum_required(VERSION 3.0.0)

project(CommonBase)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_SOURCE_DIR}/BaseCommon/include)

set(INCLUDES
include/OEMBase.h
include/TypesBase.h
include/HashBase.h
include/vector2f.h
include/vector.h
include/vector4f.h
include/quat.h
include/matrix.h
include/matrix3.h
include/mathlib.h
include/Bounds2f.h
include/Bounds3f.h
)

set(SRC
src/empty.cpp
src/vector2f.cpp
src/vector.cpp
src/vector4f.cpp
src/quat.cpp
src/matrix.cpp
src/matrix3.cpp
src/mathlib.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDES} ${SRC})

set_target_properties( ${PROJECT_NAME} PROPERTIES
					RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
					RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					LIBRARY_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
					)