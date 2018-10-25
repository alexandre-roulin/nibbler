.PHONY: all clean fclean re

#==========EXECUTABLES==========#

EXE= td

#=======COMPILER AND FLAGS======#

CXX= clang++
CXXFLAGS= -Wall -Wextra -Werror -Wvla -std=c++14 -fsanitize=address -g3 -MMD
BREW_LIB= -L ~/.brew/lib/ -lSDL2 -lSDL2_image
#============HEADERS============#

HEADER_DIR= include/
HEADER_ALL_DIR= ./include/ ~/.brew/include/

HEADER = $(addprefix $(HEADER_DIR), $(HEADER_FILE))

HEADER_FILE = IDisplay.hpp \
Vector2D.tpp \
DisplaySdl.hpp \
stb_image.h \

#============COLORS=============#

RED=\033[1;31m
GREEN=\033[1;32m
EOC=\033[0m

#==============SRC=============#

SRC_DIR = ./src/

SRC_FILE= main.cpp \
DisplaySdl.cpp

SRC = $(addprefix $(SRC_DIR), $(SRC_FILE))


OBJ_DIR = ./obj/
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_FILE))
OBJ_FILE= $(SRC_FILE:.cpp=.o)

DEPENDS= $(OBJ:.o=.d)

#=============RULES=============#

all: $(EXE)

lol:
	@echo $(addprefix "-I ", $(HEADER_ALL_DIR))

$(EXE): $(OBJ_DIR) $(OBJ)
	@echo "\t$(EXE)'s objects compiled"
	@$(CXX) $(CXXFLAGS) $(OBJ) $(BREW_LIB) -o $(EXE)
	@echo "executable $(GREEN)$(EXE)$(EOC) created"
	@echo "$(GREEN)---------->>>$(EOC)"

$(OBJ_DIR) :
	@mkdir $(OBJ_DIR)
	@mkdir $(OBJ_DIR)/instruction

$(OBJ_DIR)%.o: $(addprefix $(SRC_DIR), %.cpp) Makefile
	@$(CXX) $(CXXFLAGS) -c $< -o $@ $(addprefix -I , $(HEADER_ALL_DIR))
	@printf "$(GREEN)+$(EOC)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[x]$(EOC) $(EXE)'s objects cleaned"

fclean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)[x]$(EOC) $(EXE)'s objects cleaned"
	@rm -f $(EXE)
	@echo "executable $(RED)$(EXE)$(EOC) removed"
	@echo "$(RED)<<<----------$(EOC)"

re: fclean all

-include $(DEPENDS)
