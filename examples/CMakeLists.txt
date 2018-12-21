include_directories(${l_system_SOURCE_DIR}/include)

add_executable(algae algae.cpp)
#target_link_libraries(x ${LIBS})

add_executable(point point.cpp)

add_executable(repl repl.cpp)

add_executable(param param.cpp)

add_executable(param_system param_system.cpp)
