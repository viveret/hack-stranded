export SEMOD

SE_SO = plugins/libStranded$(SEMOD)-ubuntu.so
CXX = g++ -D_UBUNTU

SRC=$(shell find src/$(SEMOD) -name "*.cpp")
OBJS=$(patsubst %.cpp, %.o, $(SRC))
DEPS=$(patsubst %.o, %.d, $(OBJS))

SE_CXX_FLAGS_MOD =
SE_MOD_LIBS =
SE_MOD_FLAGS =
include src/$(SEMOD)/Makefile

all: ubuntu

#ubuntu: SE_SO = $(SEMOD)/libse$(SEMOD)-ubuntu.so
ubuntu: $(SE_SO)
	

android: plugins/libStranded$(SEMOD)-android.so

plugins/libStranded$(SEMOD)-android.so:
	~/bin/android-ndk-r9c/ndk-build SEMOD=$(SEMOD) NDK_DEBUG=1 --jobs -C android 
	mv android/libs/armeabi-v7a/*.so plugins/
	

<<<<<<< HEAD
$(SE_SO): SE_INC_DIR := -Isrc/$(SEMOD) -I../Survivor_Universe/Survivor_Engine/project/include -Isrc -I/usr/include/freetype2 -I../Survivor_Universe/Survivor_Engine/project/include_modules
=======
$(SE_SO): SE_INC_DIR := -Isrc/$(SEMOD) -I../Survivor_Universe/Survivor_Engine/project/include -Isrc -I../Survivor_Universe/Survivor_Engine/project/include_modules
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1
$(SE_SO): SE_CXX_FLAGS := -c -O0 -g3 -std=c++11 -rdynamic -fPIC -shared -DSE_PHYSICS_2D
$(SE_SO): SE_NOTIF_FLAGS := -Wall -Wextra -MMD -MP -Wno-comment
$(SE_SO): SE_BUILD


SE_BUILD: $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(SE_NOTIF_FLAGS) -shared $(OBJS) $(LIBS) $(SE_MOD_LIBS) -o $(SE_SO)

%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(SE_NOTIF_FLAGS) $(SE_INC_DIR) $(SE_CXX_FLAGS) $(SE_CXX_FLAGS_MOD) $(SE_MOD_FLAGS) $< -o $@
	@echo ' '

%.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(SE_NOTIF_FLAGS) $(SE_INC_DIR) $(SE_CXX_FLAGS) $(SE_CXX_FLAGS_MOD) $(SE_MOD_FLAGS) $< -o $@
	@echo ' '

clean:
	rm -f $(OBJS)
	rm -f $(DEPS)
