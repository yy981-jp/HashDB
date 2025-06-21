cmake_minimum_required(VERSION 3.16)
project(HashDB)
add_compile_options(-fdiagnostics-color=always)

set(CMAKE_CXX_STANDARD 23)

# Qt用の自動MOCを有効にする
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# ソースファイル
set(SOURCES
	hashdb.cpp
	def.cpp
	utility.cpp
)

# ヘッダーファイル
set(HEADERS
)

add_compile_options(-fexec-charset=cp932)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "C:/msys64/mingw64/include/qt6")

# Qt6のライブラリをリンク
target_link_libraries(${PROJECT_NAME} PRIVATE
	crypto
)
