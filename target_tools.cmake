cmake_minimum_required(VERSION 3.0.0)

FILE(TO_CMAKE_PATH ${G_BIN_DIR} CMAKE_G_BIN_DIR)

set( ENABLE_GTEST ON )

_thirdparty_add(gtest SOURCE_DIR utils/gtest-1.7.0)