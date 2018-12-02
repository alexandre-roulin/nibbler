

set(command "make;-j")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-install-out.log"
  ERROR_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-install-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-install-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "SFML install command succeeded.  See also /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-install-*.log")
  message(STATUS "${msg}")
endif()
