cmake_minimum_required(VERSION 2.6)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

project(StackAllocator)

if (MSVC)

else()
	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if (COMPILER_SUPPORTS_CXX11)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif (COMPILER_SUPPORTS_CXX0X)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else ()
		message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif ()
endif ()

set(StackAllocator_HDRS
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/Stack.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/Stack.inl
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/StackAllocator.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/StackAllocator.inl
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/ThreadSafeStack.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/ThreadSafeStack.inl
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/ThreadSafeStackAllocator.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/ThreadSafeStackAllocator.inl)

source_group(Includes FILES ${StackAllocator_HDRS})

set(StackAllocator_Private_HDRS
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/Private/IStack.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/Private/SpinLock.hpp
	${PROJECT_SOURCE_DIR}/StackAllocator/Includes/StackAllocator/Private/SpinLock.inl)

source_group(Includes\\Private FILES ${StackAllocator_Private_HDRS})	

set(HDRS
	${StackAllocator_HDRS}
	${StackAllocator_Private_HDRS})
	
set(SRCS
	${PROJECT_SOURCE_DIR}/StackAllocator/Sources/Test.cpp)
	
source_group(Sources FILES ${SRCS})

include_directories("$(PROJECT_SOURCE_DIR)/StackAllocator/Includes")

add_executable(StackAllocatorTest ${SRCS} ${HDRS})

enable_testing()

add_test(StackAllocatorUnitTests StackAllocatorTest)

set_tests_properties(StackAllocatorUnitTests
	PROPERTIES PASS_REGULAR_EXPRESSION "Success!")