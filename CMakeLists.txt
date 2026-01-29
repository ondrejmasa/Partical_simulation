# CMakeList.txt : CMake project for Partical_simulation, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Partical_simulation")

include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        release-3.4.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(SDL3)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.2         
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(glm)

# Add source to this project's executable.
add_executable (Partical_simulation "src/main.cpp" "include/app.h" "src/app.cpp" "src/ball.cpp" "include/ball.h" "include/global.h")

target_link_libraries(Partical_simulation PRIVATE 
    SDL3-shared 
    glm::glm 
)

target_include_directories(Partical_simulation PUBLIC include)

add_custom_command(TARGET Partical_simulation POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3-shared>
    $<TARGET_FILE_DIR:Partical_simulation>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Partical_simulation PROPERTY CXX_STANDARD 20)
endif()
