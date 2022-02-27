/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class spout_JNISpout */

#ifndef _Included_spout_JNISpout
#define _Included_spout_JNISpout
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     spout_JNISpout
 * Method:    init
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_spout_JNISpout_init
  (JNIEnv *, jclass);

/*
 * Class:     spout_JNISpout
 * Method:    deInit
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_deInit
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    createSender
 * Signature: (Ljava/lang/String;IIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createSender
  (JNIEnv *, jclass, jstring, jint, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    updateSender
 * Signature: (Ljava/lang/String;IIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_updateSender
  (JNIEnv *, jclass, jstring, jint, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    releaseSender
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_releaseSender
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    sendTexture
 * Signature: (IIIIZJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_sendTexture
  (JNIEnv *, jclass, jint, jint, jint, jint, jboolean, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    setReceiverName
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_setReceiverName
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    createReceiver
 * Signature: (Ljava/lang/String;[IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createReceiver
  (JNIEnv *, jclass, jstring, jintArray, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    releaseReceiver
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_releaseReceiver
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    receivePixels
 * Signature: ([I[IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_receivePixels
  (JNIEnv *, jclass, jintArray, jintArray, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    receiveTexture
 * Signature: ([IIIZJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_receiveTexture
  (JNIEnv *, jclass, jintArray, jint, jint, jboolean, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    senderDialog
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_senderDialog
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getSenderName
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_spout_JNISpout_getSenderName
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getSenderWidth
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderWidth
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getSenderHeight
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderHeight
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getSenderFps
 * Signature: (J)F
 */
JNIEXPORT jfloat JNICALL Java_spout_JNISpout_getSenderFps
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getSenderFrame
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderFrame
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    isFrameNew
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_isFrameNew
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    disableFrameCount
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_disableFrameCount
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    enableSpoutLog
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_enableSpoutLog
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogToFile
 * Signature: (Ljava/lang/String;ZJ)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogToFile
  (JNIEnv *, jclass, jstring, jboolean, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogLevel
 * Signature: (IJ)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogLevel
  (JNIEnv *, jclass, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLog
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLog
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogVerbose
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogVerbose
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogNotice
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogNotice
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogWarning
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogWarning
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogError
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogError
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    spoutLogFatal
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogFatal
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getTextureID
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getTextureID
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getMemoryShareMode
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_getMemoryShareMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getShareMode
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getShareMode
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    setAdapter
 * Signature: (IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_setAdapter
  (JNIEnv *, jclass, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    createControl
 * Signature: (Ljava/lang/String;Ljava/lang/String;FFFLjava/lang/String;J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createControl
  (JNIEnv *, jclass, jstring, jstring, jfloat, jfloat, jfloat, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    openControls
 * Signature: (Ljava/lang/String;J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_openControls
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    checkControls
 * Signature: ([Ljava/lang/String;[I[F[Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_checkControls
  (JNIEnv *, jclass, jobjectArray, jintArray, jfloatArray, jobjectArray, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    openController
 * Signature: (Ljava/lang/String;J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_openController
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    closeControls
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_closeControls
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    createSenderMemory
 * Signature: (Ljava/lang/String;IIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createSenderMemory
  (JNIEnv *, jclass, jstring, jint, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    updateSenderMemorySize
 * Signature: (Ljava/lang/String;IIJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_updateSenderMemorySize
  (JNIEnv *, jclass, jstring, jint, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    writeSenderString
 * Signature: (Ljava/lang/String;J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_writeSenderString
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    closeSenderMemory
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_closeSenderMemory
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    lockSenderMemory
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_spout_JNISpout_lockSenderMemory
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    unlockSenderMemory
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_unlockSenderMemory
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    setFrameSync
 * Signature: (Ljava/lang/String;J)V
 */
JNIEXPORT void JNICALL Java_spout_JNISpout_setFrameSync
  (JNIEnv *, jclass, jstring, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    waitFrameSync
 * Signature: (Ljava/lang/String;IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_waitFrameSync
  (JNIEnv *, jclass, jstring, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    writeMemoryBuffer
 * Signature: (Ljava/lang/String;Ljava/lang/String;IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_writeMemoryBuffer
  (JNIEnv *, jclass, jstring, jstring, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    readMemoryBuffer
 * Signature: (Ljava/lang/String;IJ)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_spout_JNISpout_readMemoryBuffer
  (JNIEnv *, jclass, jstring, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    createMemoryBuffer
 * Signature: (Ljava/lang/String;IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createMemoryBuffer
  (JNIEnv *, jclass, jstring, jint, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    deleteMemoryBuffer
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_deleteMemoryBuffer
  (JNIEnv *, jclass, jlong);

/*
 * Class:     spout_JNISpout
 * Method:    getMemoryBufferSize
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_spout_JNISpout_getMemoryBufferSize
  (JNIEnv *, jclass, jstring, jlong);

#ifdef __cplusplus
}
#endif
#endif
