#if !defined(SE_JAVA_MAIN) && defined(_BUILD_SO)
#define SE_JAVA_MAIN

#include <jni.h>


#ifdef __cplusplus
        extern "C" {
#endif

	void SEJavaMain( int argc, char** argv );
//        void Java_Main_SE_Java_Main();

	JNIEXPORT void JNICALL Java_SEMain_SEJavaMain( JNIEnv* env, jobject object, jobjectArray stringArray );

#ifdef __cplusplus
        }
#endif


#endif
