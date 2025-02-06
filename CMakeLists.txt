cmake_minimum_required(VERSION 3.5)

project(match_maker LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /bigobj")
endif(MSVC)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add log support
find_package(spdlog CONFIG REQUIRED)
INCLUDE_DIRECTORIES(${spdlog_include_dirs})

find_package(nlohmann_json CONFIG REQUIRED)


file(GLOB PROJECT_SRCS  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB PROJECT_HEADERS  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
add_library(${PROJECT_NAME} ${PROJECT_SRCS} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json spdlog::spdlog)

foreach(p LIB BIN INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		string(TOLOWER ${p} lower_p)
        set(${var} "${CMAKE_INSTALL_PREFIX}/${lower_p}")
	endif()
endforeach()

export(TARGETS ${PROJECT_NAME}
     FILE ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Targets.cmake)

file(WRITE
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    "include(\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake)\n")

install(FILES
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
   ARCHIVE  DESTINATION lib
   LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
  )

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

add_subdirectory(test)

