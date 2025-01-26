cmake_minimum_required( VERSION 3.30 )

project( MOS6502Emu )

if(MSVC)
	add_compile_options(/MP)				#Use multiple processors when building
	add_compile_options(/W4 /wd4201 /WX)	#Warning level 4, all warnings are errors
else()
	add_compile_options(-W -Wall -Werror) 	#All Warnings, all warnings are errors
endif()

set  (MOS6502_SOURCES
	"src/mos6502.h"
	"src/mos6502.cc"
    "src/main.cc")
		
source_group("src" FILES ${MOS6502_SOURCES})
		
add_library( MOS6502Emu ${MOS6502_SOURCES} )

target_include_directories ( MOS6502Emu PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_include_directories ( MOS6502Emu PRIVATE "${PROJECT_SOURCE_DIR}/src")
