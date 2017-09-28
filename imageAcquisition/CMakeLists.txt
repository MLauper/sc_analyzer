# Setup CUDA
include(FindCUDA)
if (NOT CUDA_FOUND)
	message( FATAL_ERROR "CUDA not found. Please install CUDA or disable CUDA in this project." )
endif ()
if (NOT CUDA_VERSION_MAJOR EQUAL 9)
	message( WARNING "CUDA is not in version 9. This is not a tested scenario." )
endif()
include_directories ("${CUDA_INCLUDE_DIRS}")

# Setup ArrayFire
FIND_PACKAGE(ArrayFire REQUIRED)
if (NOT ArrayFire_CUDA_FOUND)
	message( FATAL_ERROR "ArrayFire CUDA Backend not found. Please install CUDA and ArrayFire." )
endif ()
INCLUDE_DIRECTORIES(${ArrayFire_INCLUDE_DIRS})

cuda_add_library(
	lib_image_acquisition
	lib_image_acquisition.cu
	lib_image_acquisition.h
)
target_link_libraries (
	lib_image_acquisition
	#${ArrayFire_LIBRARIES}
	${CUDA_LIBRARIES}
)

cuda_add_executable(
	image_acquisition
	image_acquisition.cu
	image_acquisition.h
)
add_dependencies(
	image_acquisition
	lib_image_acquisition
)
target_link_libraries (
	image_acquisition
	lib_image_acquisition
	${ArrayFire_LIBRARIES}
	${CUDA_LIBRARIES}
)
	
if (USE_GTEST)
	cuda_add_executable(
		lib_image_acquisition_unittest
		lib_image_acquisition_unittest.cu
	)

	add_dependencies(
		lib_image_acquisition_unittest
		lib_image_acquisition
		gtest
	)

	target_link_libraries(
		lib_image_acquisition_unittest
		lib_image_acquisition
		gtest_main
		${ArrayFire_LIBRARIES}
		${CUDA_LIBRARIES}
	)

endif()
