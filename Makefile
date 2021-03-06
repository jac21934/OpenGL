CXX := g++
CXXFLAGS := -std=c++11 -Wall -O3
SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include
LIB_DIR = lib

EXE = test
LDLIBS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lassimp
LDFLAGS =-L$(LIB_DIR) -Wl,-rpath=$(LIB_DIR) # -L/home/jacob/Downloads/assimp-4.1.0/lib/ -Wl,-rpath=/home/jacob/Downloads/assimp-4.1.0/lib/

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
COBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INC= -I$(INCLUDE_DIR) -I/usr/local/include
HEADERS = $(wildcard $(INCLUDE_DIR)/%.h)
all: directories $(EXE) 

directories: $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)


$(EXE): $(OBJ) obj/glad.o 
	$(CXX) $(CXXFLAGS) $(INC) $(LDFLAGS)  $^ $(LDLIBS) -o $@




$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp 
	$(CXX) $(CXXFLAGS)  $(INC) -c $< -o $@

$(OBJ_DIR)/glad.o: $(SRC_DIR)/glad.c
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@	



clean:
	$(RM) $(EXE) $(OBJ)


.PHONY: all clean directories



