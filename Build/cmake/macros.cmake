#macro(SET_BIN_OUTPUT_FOLDER proj out)#set_target_properties(${proj} PROPERTIES #RUNTIME_OUTPUT_DIRECTORY_DEBUG ${out}#LIBRARY_OUTPUT_DIRECTORY_DEBUG ${out}#RUNTIME_OUTPUT_DIRECTORY_RELEASE ${out}#IBRARY_OUTPUT_DIRECTORY_RELEASE ${out})#endmacro(SET_BIN_OUTPUT_FOLDER)macro(debug msg)    message(STATUS "DEBUG ${msg}")endmacro()macro(debugValue variableName)    debug("${variableName}=\${${variableName}}")endmacro()