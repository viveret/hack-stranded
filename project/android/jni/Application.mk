#APP_STL := stlport_static
APP_PLATFORM			:= android-19
APP_ABI               := armeabi-v7a
APP_OPTIM             := debug
NDK_DEBUG             := 1

#NDK_TOOLCHAIN_VERSION := 4.6
APP_STL					:= gnustl_static
APP_CPPFLAGS			:= -fexceptions -frtti -std=c++11

#APP_LDLIBS				:= -pthread -lBulletDynamics -lBulletCollision -lLinearMath \
#							-lz -lzip -ldl -lGL -lGLESv1_CM -llog
