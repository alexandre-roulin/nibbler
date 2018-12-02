

set(command "/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake;-P;/Users/aroulin/nibbler/cmake_modules/sfml/tmp/SFML-gitclone.cmake")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-out.log"
  ERROR_FILE "/Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "SFML download command succeeded.  See also /Users/aroulin/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-*.log")
  message(STATUS "${msg}")
endif()
