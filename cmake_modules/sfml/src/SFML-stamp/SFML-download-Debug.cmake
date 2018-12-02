

set(command "/usr/local/Cellar/cmake/3.12.2/bin/cmake;-P;/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/tmp/SFML-gitclone.cmake")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-out.log"
  ERROR_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "SFML download command succeeded.  See also /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-download-*.log")
  message(STATUS "${msg}")
endif()
