

set(command "${make}")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-build-out.log"
  ERROR_FILE "/Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "SFML build command succeeded.  See also /Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-build-*.log")
  message(STATUS "${msg}")
endif()
