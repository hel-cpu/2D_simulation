cmake_minimum_required(VERSION 3.31.6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(ProjectName)

file(GLOB_RECURSE MY_SOURCES CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES CMAKE_CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/includes/*.h"
)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES} ${HEADER_FILES})

target_include_directories(${CMAKE_PROJECT_NAME} 
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PUBLIC
        GLEW::GLEW
        glm::glm
        glfw
        OpenGL::GL
)

# Source files listed in output
message(STATUS "Sources found:")
foreach(file ${MY_SOURCES})
    message(STATUS " - ${file}")
endforeach()

# Copy shader files to the build directory
set(RESOURCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(RESOURCE_DEST_DIR "${CMAKE_BINARY_DIR}/resources")

# Create destination directory (idempotent operation)
file(MAKE_DIRECTORY "${RESOURCE_DEST_DIR}")

# Define subdirectories to copy (shaders and textures)
set(RESOURCE_SUBDIRS shaders textures)
    
# Copy files recursively for each subdirectory
foreach(SUBDIR ${RESOURCE_SUBDIRS})
    set(SRC_DIR "${RESOURCE_SOURCE_DIR}/${SUBDIR}")

    if(EXISTS "${SRC_DIR}")
        file(MAKE_DIRECTORY "${RESOURCE_DEST_DIR}/${SUBDIR}")

        add_custom_command(
            TARGET ${CMAKE_PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${SRC_DIR}"
                "${RESOURCE_DEST_DIR}/${SUBDIR}"
            COMMENT "Copying ${SUBDIR} files to build directory..."
        )
    else()
        message(WARNING "Source directory ${SRC_DIR} does not exist. Skipping copy.")
    endif()
endforeach()

target_compile_definitions(${CMAKE_PROJECT_NAME} PUBLIC
    RESROUCE_DIR="${RESOURCE_DEST_DIR}"
    SHADER_DIR="${RESOURCE_DEST_DIR}/shaders/"
    TEXTURE_DIR="${RESOURCE_DEST_DIR}/textures/"
) 