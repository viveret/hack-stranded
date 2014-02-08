PROGRAM = Survivor_Engine

# Compiler
CXX := g++

# Compiler options during compilation
CXXFLAGS := -c -O0 -g3 -std=c++11
NOTIF_FLAGS := -Wall -Wextra -MMD -MP -Wno-comment

# Libraries for linking
LIBS := -lX11 -lboost_system -lboost_thread -lGL -lGLU -lGLEW -lIL -lILU -lILUT -lfreetype -lassimp -lbass -lBulletDynamics -lBulletCollision -lLinearMath -lnoise -llua5.2 -lz -lzip

MODULES = # For all the file names
# Include all other makefiles
include src/Makefile
# Source files to add
SOURCES = $(MODULES:%=src/%.cpp) 
# Objects to add
OBJECTS = $(MODULES:%=obj/%.o) 

all: build/Survivor_Engine

build/Survivor_Engine: $(OBJECTS)
	$(CXX) $(NOTIF_FLAGS) $(OBJECTS) $(LIBS) -o $@

#$(OBJECTS): $(SOURCES)
obj/%.o: src/%.cpp
	$(CXX) $(NOTIF_FLAGS) $(CXXFLAGS) $< -o $@
	@echo ' '

obj/%.o: src/%.c
	$(CXX) $(NOTIF_FLAGS) $(CXXFLAGS) $< -o $@
	@echo ' '

clean:
	@find obj/ -name \*.o -delete
	@find obj/ -name \*.d -delete

