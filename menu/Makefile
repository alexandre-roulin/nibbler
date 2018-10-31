.PHONY: all clean fclean re

#==========EXECUTABLES==========#

EXE= menu

#=======COMPILER AND FLAGS======#

CXX= clang++
CXXFLAGS= -Wall -Wextra -Werror -Wvla -std=c++14 -fsanitize=address -g3 -MMD
#============HEADERS============#

HEADER_DIR= ./include/

#============COLORS=============#

RED=\033[1;31m
GREEN=\033[1;32m
EOC=\033[0m

#==============SRC=============#

SRC_DIR = ./src/

SRC_FILE= main.cpp

SRC = $(addprefix $(SRC_DIR), $(SRC_FILE))


OBJ_DIR = ./obj/
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILE))
OBJ_FILE= $(SRC_FILE:.cpp=.o)

DEPENDS= $(OBJ:.o=.d)

#==========AGNOSTIC==========#

UNAME := $(shell uname)

ADDITIONNAL_LIB=
ifeq ($(UNAME), Linux)
ADDITIONNAL_LIB += -L /usr/lib/
HEADER_DIR += -I /usr/include/ -I imgui-sfml/
endif
ifeq ($(UNAME), Darwin)
ADDITIONNAL_LIB += -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
ADDITIONNAL_LIB += -L ~/.brew/lib/  -rpath ~/.brew/lib
HEADER_DIR += ~/.brew/include/ -I imgui-sfml/
endif

ADDITIONNAL_LIB += -L imgui-sfml/ -limgui-sfml -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
HEADER_DIR += -I imgui-sfml/ -I ../externlib/commun/

#=============RULES=============#

all: $(EXE)

$(EXE): $(OBJ_DIR) $(OBJ)
	@echo "\t$(EXE)'s objects compiled"
	$(CXX) $(CXXFLAGS) $(OBJ) $(ADDITIONNAL_LIB) -o $(EXE)
	@echo "executable $(GREEN)$(EXE)$(EOC) created"
	@echo "$(GREEN)---------->>>$(EOC)"

$(OBJ_DIR) :
	@mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: $(addprefix $(SRC_DIR), %.cpp) Makefile
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(addprefix -I , $(HEADER_DIR))
	@printf "$(GREEN)+$(EOC)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[x]$(EOC) $(EXE)'s objects cleaned"

fclean: clean
	@rm -f $(EXE)
	@echo "executable $(RED)$(EXE)$(EOC) removed"
	@echo "$(RED)<<<----------$(EOC)"

re: fclean all

-include $(DEPENDS)
