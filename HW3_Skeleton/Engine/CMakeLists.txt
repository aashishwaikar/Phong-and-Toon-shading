project(engine_project)

file(GLOB ENGINE_SRC "src/*.cpp")
file(GLOB ENGINE_HEADERS "include/*.hpp")

if(MSVC)
    source_group("Header Files" FILES ${ENGINE_HEADERS})

    source_group("Source Files" FILES ${ENGINE_SRC})
endif()

add_library(Engine ${ENGINE_SRC} ${ENGINE_HEADERS})

target_include_directories(Engine
    PUBLIC include
    PRIVATE src
)

target_link_libraries(Engine GLEW::GLEW ${OPENGL_LIBRARY} glm)

set_target_properties(
  Engine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")