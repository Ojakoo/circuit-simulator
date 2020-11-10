file(GLOB SOURCES "*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main
  PUBLIC
    ImGui-SFML::ImGui-SFML
)

include(Install.cmake)