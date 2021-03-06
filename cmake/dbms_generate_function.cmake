
function(generate_function_register FUNCTION_AREA)
  #set(FUNCTION_AREA_H Functions${FUNCTION_AREA}.h)

  foreach(FUNCTION IN LISTS ARGN)
    configure_file (registerFunction.h.in register${FUNCTION}.h)
    configure_file (registerFunction.cpp.in register${FUNCTION}.cpp)
    set(REGISTER_HEADERS "${REGISTER_HEADERS} #include \"register${FUNCTION}.h\"\n")
    set(REGISTER_FUNCTIONS "${REGISTER_FUNCTIONS} register${FUNCTION}(factory);\n")
  endforeach()

  configure_file (Functions${FUNCTION_AREA}.cpp.in register${FUNCTION_AREA}.cpp)

endfunction()
