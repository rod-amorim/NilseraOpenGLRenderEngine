cmake_minimum_required(VERSION 3.30)
project(NilseraOpenGLRenderEngine)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_executable(NilseraOpenGLRenderEngine
        src/main.cpp
        src/Shader/Shader.h
        src/Shader/Shader.cpp
        libs/stb/stb_image.cpp
        src/Texture/Texture.cpp
        src/Texture/Texture.h
        src/VertexBuffer/VertexBuffer.cpp
        src/VertexBuffer/VertexBuffer.h
        src/VertexArray/VertexArray.cpp
        src/VertexArray/VertexArray.h
        src/IndexBuffer/IndexBuffer.cpp
        src/IndexBuffer/IndexBuffer.h
        src/VertexBufferLayout/VertexBufferLayout.cpp
        src/VertexBufferLayout/VertexBufferLayout.h
        src/core/Components/Renderer/Renderer.cpp
        src/core/Components/Renderer/Renderer.h
        src/core/Components/Camera/Camera.cpp
        src/core/Components/Camera/Camera.h
        src/core/GameTime.cpp
        src/core/GameTime.h
        src/core/Screen.cpp
        src/core/Screen.h
        src/core/Material.cpp
        src/core/Material.h
        src/core/Application.cpp
        src/core/Application.h
        src/core/Input.cpp
        src/core/Input.h
        src/core/GameObject.cpp
        src/core/GameObject.h
        src/core/Transform.cpp
        src/core/Transform.h
        src/core/Scene.cpp
        src/core/Scene.h
        src/core/SceneManager.cpp
        src/core/SceneManager.h
        src/core/Components/IComponent.h
        src/core/Components/Behaviour/FirstPersonCharacterController.cpp
        src/core/Components/Behaviour/FirstPersonCharacterController.h
        src/core/Components/Light/Light.cpp
        src/core/Components/Light/Light.h
        src/Scenes/TestScene.cpp
        src/Scenes/TestScene.h
        src/core/Components/Light/PointLight/PointLight.cpp
        src/core/Components/Light/PointLight/PointLight.h
        src/core/Components/Light/DirectionalLight/DirectionalLight.cpp
        src/core/Components/Light/DirectionalLight/DirectionalLight.h
        src/core/Components/Light/SpotLight/SpotLight.cpp
        src/core/Components/Light/SpotLight/SpotLight.h
        src/core/Mesh/Mesh.cpp
        src/core/Mesh/Mesh.h
        src/core/Model/Model.cpp
        src/core/Model/Model.h
        src/core/LightManager/LightManager.cpp
        src/core/LightManager/LightManager.h
        src/core/TMap/TMap.cpp
        src/core/TMap/TMap.h
        src/core/Structs/Vertex.cpp
        src/core/Structs/Vertex.h
)

target_include_directories(NilseraOpenGLRenderEngine PRIVATE ${CMAKE_SOURCE_DIR}/src)

#imgui
include_directories(${CMAKE_SOURCE_DIR}/libs/Imgui)
set(IMGUI_SOURCES
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui.cpp
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui_draw.cpp
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui_tables.cpp
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui_widgets.cpp
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/libs/Imgui/imgui_impl_glfw.cpp)
target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES})

#assimp
include_directories(${CMAKE_SOURCE_DIR}/libs/assimp/include)
link_directories(${CMAKE_SOURCE_DIR}/libs/assimp)

#Stb
include_directories(${CMAKE_SOURCE_DIR}/libs/stb)

#Glm
include_directories(${CMAKE_SOURCE_DIR}/libs/glm)

#GLEW
include_directories(${CMAKE_SOURCE_DIR}/libs/glew-2.1.0/include)
set(GLEW_STATIC_LIB ${CMAKE_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32s.lib)
target_compile_definitions(NilseraOpenGLRenderEngine PRIVATE GLEW_STATIC)
target_link_libraries(NilseraOpenGLRenderEngine PRIVATE ${GLEW_STATIC_LIB})

#GLFW
include_directories(${CMAKE_SOURCE_DIR}/libs/GLFW/include)
set(GLFW_STATIC_LIB ${CMAKE_SOURCE_DIR}/libs/GLFW/libglfw3.a)

target_link_libraries(NilseraOpenGLRenderEngine PRIVATE ${GLFW_STATIC_LIB}
        OpenGL::GL
        ${CMAKE_SOURCE_DIR}/libs/assimp/libassimp.dll.a)

add_custom_command(TARGET NilseraOpenGLRenderEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/libs/assimp/libassimp-5.dll
        $<TARGET_FILE_DIR:NilseraOpenGLRenderEngine>)

add_custom_command(TARGET NilseraOpenGLRenderEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:NilseraOpenGLRenderEngine>/res)