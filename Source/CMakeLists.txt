set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(LIBRARY_SOURCES
    ${DIR}/Geometry.h
    ${DIR}/Geometry.cpp
    ${DIR}/Renderer.h
    ${DIR}/Renderer.cpp
    ${DIR}/Camera.h
    ${DIR}/Camera.cpp
    ${DIR}/PolyLine.h
    ${DIR}/PolyLine.cpp
    ${DIR}/Circle.h
    ${DIR}/Recorder.h
    ${DIR}/Recorder.cpp
    PARENT_SCOPE
)

set(SHADER_SOURCES
    ${DIR}/Shaders/line.glsl
    PARENT_SCOPE
)

set(LIBRARY_PUBLIC_HEADERS
    ${DIR}/Geometry.h
    ${DIR}/Renderer.h
    ${DIR}/Camera.h
    ${DIR}/PolyLine.h
    ${DIR}/Circle.h
    ${DIR}/Recorder.h
    PARENT_SCOPE
)
