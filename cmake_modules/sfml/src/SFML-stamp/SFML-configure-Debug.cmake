

set(command "/usr/local/Cellar/cmake/3.12.2/bin/cmake;-DCMAKE_INSTALL_PREFIX=/Users/alexandreroulin/work/nibbler/cmake_modules/sfml;-DCMAKE_INSTALL_FRAMEWORK_PREFIX=/Users/alexandreroulin/work/nibbler/cmake_modules/sfml;-DSFML_MISC_INSTALL_PREFIX=/Users/alexandreroulin/work/nibbler/cmake_modules/sfml;-C/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/tmp/SFML-cache-Debug.cmake;-GUnix Makefiles;/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-configure-out.log"
  ERROR_FILE "/Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "SFML configure command succeeded.  See also /Users/alexandreroulin/work/nibbler/cmake_modules/sfml/src/SFML-stamp/SFML-configure-*.log")
  message(STATUS "${msg}")
endif()
