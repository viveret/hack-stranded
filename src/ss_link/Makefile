PROGRAM = Survivor_Script

# Compiler
CXX := g++

# Compiler options during compilation
CXXFLAGS := -c -O0 -g3 -std=c++11 
NOTIF_FLAGS := -Wall -Wextra -MMD -MP -Wno-comment

# Libraries for linking
LIBS := 

MODULES = # For all the file names
# Include all other makefiles
include Makefile_Sources

# Source files to add
SOURCES = $(MODULES:%=%.cpp) 
# Objects to add
OBJECTS = $(MODULES:%=obj/%.o) 

all: build/Survivor_Script

build/Survivor_Script: $(OBJECTS)
	$(CXX) $(NOTIF_FLAGS) $(OBJECTS) $(LIBS) -o $@

#$(OBJECTS): $(SOURCES)
obj/%.o: %.cpp
	$(CXX) $(NOTIF_FLAGS) $(CXXFLAGS) $< -o $@
	@echo ' '

obj/%.o: %.c
	$(CXX) $(NOTIF_FLAGS) $(CXXFLAGS) $< -o $@
	@echo ' '

clean:
	@find obj/ -name \*.o -delete
	@find obj/ -name \*.d -delete

