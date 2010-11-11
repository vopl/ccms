#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_BUILD_TYPE "RelWithDebInfo")

SET(PREFIX /home/ccms/server)
SET(OWNER "ccms")
SET(GROUP "ccms")

SET(CMAKE_USE_RELATIVE_PATH "TRUE")
SET(CMAKE_SKIP_RPATH "TRUE")
SET(CMAKE_VERBOSE_MAKEFILE TRUE)















################### boost
SET(Boost_DETAILED_FAILURE_MSG TRUE)
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(BOOST_ROOT /usr/local)
set(BOOST_INCLUDEDIR /usr/local/lib)
find_package( Boost 1.37 )


if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	message("------------ use boost " ${Boost_MAJOR_VERSION} . ${Boost_MINOR_VERSION} . ${Boost_SUBMINOR_VERSION} )
else(Boost_FOUND)
	message(FATAL_ERROR "boost 1.37 is required")
endif(Boost_FOUND)
################### boost
	

	
################### imageMagick
#SET(CMAKE_FIND_LIBRARY_PREFIXES lib "")
#SET(CMAKE_FIND_LIBRARY_SUFFIXES .so .la .a)
FIND_PACKAGE(ImageMagick)


if(ImageMagick_FOUND)
	include_directories(${ImageMagick_INCLUDE_DIRS})
	include_directories(/usr/local/include/ImageMagick)
	message("------------ use imageMagick" ${ImageMagick_INCLUDE_DIRS})
else(ImageMagick_FOUND)
	message(FATAL_ERROR "imageMagick is required")
endif(ImageMagick_FOUND)
################### boost
	

IF(CMAKE_CXX_COMPILER STREQUAL "cl")
	SET(MSVC TRUE)
ENDIF(CMAKE_CXX_COMPILER STREQUAL "cl")


IF(MSVC)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SCL_SECURE_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_CRT_NONSTDC_NO_DEPRECATE)
	ADD_DEFINITIONS(-D_SECURE_SCL=0)

	ADD_DEFINITIONS(-D_CONSOLE)
	ADD_DEFINITIONS(-DXP_WIN)

	SET(CMAKE_CXX_STANDARD_LIBRARIES "")
	SET(CMAKE_C_STANDARD_LIBRARIES "")

	SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../bin)
ELSE(MSVC)
	link_directories(/usr/local/lib)
	
	SET(CMAKE_C_FLAGS "")
	SET(CMAKE_C_FLAGS_DEBUG           "-g")
	SET(CMAKE_C_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
	SET(CMAKE_C_FLAGS_RELEASE         "-O3 -DNDEBUG")
	SET(CMAKE_C_FLAGS_RELWITHDEBINFO  "-O2 -g")

	SET(CMAKE_CXX_FLAGS "")
	SET(CMAKE_CXX_FLAGS_DEBUG           "-g")
	SET(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELEASE         "-O3 -DNDEBUG")
	SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
	
ENDIF(MSVC)




#######################################################################################
#######################################################################################
#######################################################################################
#######################################################################################
#######################################################################################
#######################################################################################
IF(MSVC)
	MACRO(PCH_KEY2FILENAME pchfile key header)
		GET_FILENAME_COMPONENT(filename ${header} NAME_WE)
		SET(pchfile "$(IntDir)/${filename}.pch")
		GET_FILENAME_COMPONENT(key ${header} NAME)
	ENDMACRO(PCH_KEY2FILENAME)


	MACRO(CREATE_PCH target header srcfile)
		PCH_KEY2FILENAME(pchfile key ${header})

		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES COMPILE_FLAGS "/Yc${key} /Fp${pchfile}")
#		MESSAGE("precompiled ${key} created by ${srcfile}")
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header)
		PCH_KEY2FILENAME(pchfile key ${header})
	
		FOREACH(i ${ARGN})
			SET_SOURCE_FILES_PROPERTIES(${i} PROPERTIES COMPILE_FLAGS "/Yu${key} /Fp${pchfile}")
#			MESSAGE("precompiled ${key} used in ${i}")
		ENDFOREACH(i)
	ENDMACRO(USE_PCH)
ELSE(MSVC)
	MACRO(PCH_KEY2FILENAME pchfile key header)
		GET_FILENAME_COMPONENT(filename ${header} NAME_WE)
		GET_FILENAME_COMPONENT(path ${header} PATH)
		SET(pchfile "${path}/${filename}.h.gch")

		GET_FILENAME_COMPONENT(key ${header} NAME)
	ENDMACRO(PCH_KEY2FILENAME)

	MACRO(CREATE_PCH target header srcfile)
	
		PCH_KEY2FILENAME(pchfile key ${header})
		
		GET_DIRECTORY_PROPERTY(INCS INCLUDE_DIRECTORIES)
		SET(IINCS "")
		FOREACH(i ${INCS})
			SET(IINCS "${IINCS} -I${i}")
		ENDFOREACH(i)

		GET_DIRECTORY_PROPERTY(DEFS DEFINITIONS)

		#SET(PCH_BUILD_COMMAND "${CMAKE_CXX_COMPILER} -fPIC ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UC}} ${IINCS} ${DEFS} ${header}")
		string(TOUPPER "${CMAKE_BUILD_TYPE}" BUILD_TYPE_UC)
		SET(PCH_BUILD_COMMAND "${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS_${BUILD_TYPE_UC}} ${IINCS} ${DEFS} ${header}")
		
		SET(target_pch ${target}_build_pch)

		ADD_CUSTOM_COMMAND(OUTPUT ${pchfile} COMMAND sh -c "${PCH_BUILD_COMMAND}" DEPENDS ${header})

		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES COMPILE_FLAGS "-include ${header} -Winvalid-pch" )
		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES OBJECT_DEPENDS ${pchfile} )
	ENDMACRO(CREATE_PCH)

	MACRO(USE_PCH target header srcfile)
		PCH_KEY2FILENAME(pchfile key ${header})
		
		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES COMPILE_FLAGS "-include ${header} -Winvalid-pch" )
		SET_SOURCE_FILES_PROPERTIES(${srcfile} PROPERTIES OBJECT_DEPENDS ${pchfile} )
	ENDMACRO(USE_PCH)
ENDIF(MSVC)
#######################################################################################








INCLUDE_DIRECTORIES(
	.
	../../include
	../../src
	../../left/sqlite-amalgamation-3_6_23_1
)


IF(UNIX)
	INCLUDE_DIRECTORIES(
		/usr/local/include
	)
ELSE(UNIX)
	# INCLUDE_DIRECTORIES(
		# ../../left/js/include
	# )
ENDIF(UNIX)






SET(CMAKE_INSTALL_PREFIX ${PREFIX})
SET(CMAKE_INSTALL_OWNER ${OWNER})
SET(CMAKE_INSTALL_GROUP ${GROUP})

