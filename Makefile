all:
	@if [ -a ./build/Makefile ] ; \
		then \
			make -j 4 -C build/ ; \
		else \
			brew install cmake ; \
			mkdir -p build ; cd build ; cmake .. -DINSTALL=ON -DCMAKE_CXX_COMPILER="/usr/bin/g++" ; make -j 4 ; \
	 fi;

clean:
	@/bin/rm -rf build/CMakefiles build/dynamicLibrariesSources build/extern build/nibblerSources build/cmake_install.cmake build/CMakeCache.txt build/Project.cbp

fclean:
	@/bin/rm -rf build nibbler

re: fclean all
