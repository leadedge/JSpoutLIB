/*

        JSpoutLIB.cpp : Defines the functions for the JNI dll used for the Processing Library

		This library is an interface between C++ and Java to create 
		JNI dll files for use with the SpoutProcessing library project.

		The project produces "JSpoutLIB.dll". Both 64 bit and 32 bit dll files
		"JNISpout_64.lib" and "JNISpout_32.lib" respectively.
		These should be copied	to the "eclipse-workspace\spout\lib\" folder.

		The header "spout.JNISpout.h is produced by the Eclipse "spout" project
		from the definitions in "JNISpout.java".

		If "JNISpout.java" is ever changed, the modified file must be used.
		
		In the package "spout"
			public class JNISpout (JNISpout.java)
			public class Spout (Spout.java)

		The JNISpout class loads the required libary accoring to "sunDataModel"
		sunDataModel.equals("32") and sunDataModel.equals("64") respectively
		The Spout class initializes an object of the JNISpout class and creates a pointer to it.
		This pointer is passed to each JNISpout function, seen as the "jlong ptr" argument below.


		- - - - - - - - - - - - - - - - - - - - - - - - - - 
		Copyright (c) 2014-2023, Lynn Jarvis. All rights reserved.

		Redistribution and use in source and binary forms, with or without modification, 
		are permitted provided that the following conditions are met:

		1. Redistributions of source code must retain the above copyright notice, 
		   this list of conditions and the following disclaimer.

		2. Redistributions in binary form must reproduce the above copyright notice, 
		   this list of conditions and the following disclaimer in the documentation 
		   and/or other materials provided with the distribution.

		THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"	AND ANY 
		EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
		OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE	ARE DISCLAIMED. 
		IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
		INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
		PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
		INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
		LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
		OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
		- - - - - - - - - - - - - - - - - - - - - - - - - - 


		06-08-14 - conversion to Spout SDK
				 - some simplification of spout.pde
		14-08-14 - TODO - test DX11 - create a DX9 sender and receiver for initial release
				 - finally JSpout should have an optional DirectX mode flag
		16.08.14 - Tested endian for Receivetexture into Processing pixels (JAVA is big endian)
		18.08.14 - Set to DX9 for a receiver for testing
		20.08.14 - Changed back to pixel method for ReadTexture for testing
		24.08.14 - recompiled with MB sendernames class revision
		05.09.14 - compiled with revised SDK + cleanup
		30.09.14 - Java_spout_JNISpout_ReadTexture uses "ReceiveImage"
				 - to transfer shared texture to Processing image pixels
				 - DirectX 11
		04.02.15 - Fixed error in InitReceiver. Sender object used instead of receiver object for 
		           memoryshare test prevented sketch to stop and java was not terminated on close.
		05.02.15 - changed InitSender to protect against null string copy
				 - included FBO in call to SendTexture
		06.02.15 - SDK recompile - added UseD3D9 define to compile for both DirectX 9 or DirectX 11
		29.06.15 - Added Spout Controls
		03.07.15 - Checked for null name and text for release env in Java_spout_JNISpout_CheckControls
		07.07.15 - Converted VS2012 project to VS2010
				 - Compiled with JDK 8.45
		17.07.15 - Recomplile Win32 with SpoutControls class revisions. No changes made to JSpout functions.
		22.07.15 - Included "CreateControl"
		27.07.15 - Used pixel transfer method for Java_spout_JNISpout_ReadTexture due to problems with Intel 4000 graphics
				 - recompiled Win32 and x64 after work on Max externals
		01.08.15 - Recompile Win32 for 2.004 release VS2010 /MT UseD3D9 undefined
		01.08.15 - Recompile x64 for 2.004 release VS2012 /MT UseD3D9 undefined
		25.09.15 - Recompile x64 for 2.005 memoryshare VS2012
		11.10.15 - removed user option for memoryshare - 2.005
		15.10.15 - Added ReceiveTexture, DrawTexture and GetTextureID
		23.10.15 - Added Java_spout_JNISpout_WriteFrameNumber to write a dword frame number to the texture (removed)
		31.10.15 - Introduced bIsSending and bIsReceiving for shared memory functions common to sender and receiver
				 - Added shared memory buffer create, close, lock, unlock, write and read
		04.11.15 - Removed ReceiveImage - now done via the image texture and ReceiveTexture
		18.11.15 - added CheckReceiver so that DrawSharedTexture can be used by a receiver
		15.12.15 - change all function names to start with lower case
		16.12.15 - added bIsSending flag to getTextureID and getMemoryShareMode
		16.12.15 - reorganised code into SENDER and RECEIVER - still the same class
		17.01.16 - Rebuild for "SpoutControls::OpenControls" added host executable path to registry "ControlPath"
		30.01.16 - Change name from JSpout to JNISpout
		02.01.16 - debug remove glGetIntegerV
		05.02.16 - restore glGetIntegerV
				 - added debug SaveOpenGLstate and RestoreOpenGLstate for DrawSharedTexture
		08.02.16 - remove debug Save and Restore OpenGL state
		01.03.16 - added "updateSender"
		06.03.16 - added "getSpoutSenderName"
				 - introduced JNISpout_init to create a Spout object and return a pointer to it
				 - change to all functions to use an object pointer
		30.03.16 - rebuild for 2.005 release - VS2012 /MT
		08.04.16 - restored ReceiveImage due to pixels not being updated by ReceiveTexture
		18.04.16 - Changed name of function to ReceivePixels
		10.05.16 - Fixed closeControls - missing pointer argument
		12.05.16 - rebuild for Processing Library 1.004 - VS2012 /MT 64 and 32bit
		02.06.16 - rebuild for Processing Library 1.005 - VS2012 /MT 64 and 32bit (Spout 2.005 June release)
		07.07.16 - Updated ReceiveImage with invert flag
				   rebuild with Spout SDK updates
		15.01.17 - Add host fbo arg to drawTexture
		26.01.17 - Rebuild for Spout 2.006 - VS2012 /MT
		08.02.17 - Rebuild with current SDK files - VS2012 /MT
		03.07.18 - Change to VS2017 / Windows 10
				   Correct spelling of CheckGLerror function
				   Remove references to Glut library - not needed
				   Remove build copy - TODO - restore
				   Change to JDK 8.91
		26.11.18 - Changes for 2.007
						getSenderWidth
						getSenderHeight
						getSenderFps
						getSenderFrame
						disableFrameCount
		04.06.19 - Rebuild 32/64bit for Spout 2.007 - VS2017 /MT
		06-06-19 - Re-build for 256 max senders - 32 bit and 64 bit - VS2017 /MT
		13-10-19 - Re-build for revised Spout SDK - 32 bit and 64 bit - VS2017 /MT
		27-11-19 - Re-build for revised Spout SDK - 32 bit and 64 bit - VS2017 /MT
		22.01.20 - changed name GetSharedTextureID
				   Re-build for revised Spout SDK - 32 bit and 64 bit - VS2017 /MT
		16.04.20 - add unreferenced parameters to setAdapter
		19.06.20 - Re-build 32/64 - VS2017 /MT
		06.07.20 - Re-build 32/64 - VS2017 /MT
		24.12.20 - Update for SpoutGL 2.007
				   NOTE : setSenderName is defined within the Processing library
				   and the SpoutGL function is not used.
				   setReceiverName is also defined but the libary function is also used.
				   NOTE: do not enable the legacyOpenGL define in SpoutCommon.h for build
				   or there is an OpenGL error 1280 at bottom draw due to use of
				   glGetString in "isExtensionSupported".
				   Remove drawTexture
				   Add setReceiverName
		12.01.21 - Release orphaned senders in SpoutSDK SelectSenderPanel
				   and sendernames CreateSender
		13.01.21 - Remove CleanSenders in sendernames::CreateSender
		24.01.21 - Re-build 32/64 - VS2017 /MT
		21.05.21 - Re-build 32/64 - VS2017 /MT with latest SpoutGL code
		02.06.21 - Create Processing library data examples
		08.06.21 - Finalise shared memory data transfer functions
		14.06.21 - Rebuild 32/64 - VS2017 /MT with latest SpoutGL code
				   Rebuild Java Processing library and test.
		22.02.22 - Rebuild 32/64 - VS2017 /MT with latest SpoutGL code
				   Master updated from Beta Vers 2.007.007 - GitHub release 2.007h
		25.02.22 - Remove checkReceiver
		26.02.22 - readMemoryBuffer - Clear buffer for return if ReadMemoryBuffer is not successful
		27.02.22 - Create GitHub repo
				   Current SpoutGL files must be copied to the project "SpoutGL" folder.
		26.03.22 - Change to VS2022, update SpoutGL files from VS2022 folder
				   Rebuild Win32/x64 - VS2022 /MT
		08.11.22 - Rebuild with updated SpoutGL files
				   Retain Shared memory for SpoutControls
		18.12.22 - Minor changes for code analysis (size_t/int)
		26.12.22 - Update SpoutGL and SpoutControls files 
		06.01.23 - Update SpoutGL to 2.007.009
				   Rebuild x86/x64 - VS2022 /MT


*/
#include "malloc.h"
#include <direct.h>

#include "SpoutGL/Spout.h"
#include "Spout/SpoutControls.h"
#include "Spout/SpoutMemoryShare.h"
#include "spout_JNISpout.h"

using namespace std;

float vpdim[4];

SpoutControls spoutcontrols;  // Spout controls object - single instance only
spoutMemoryShare memoryshare; // For shared memory functions


// LOCAL
void SaveOpenGLstate();
void RestoreOpenGLstate();
void CheckGLerror();

// INITIALIZE
// init

// SENDER
// setSenderName
// createSender
// updateSender
// releaseSender
// sendTexture
// createSenderMemory
// updateSenderMemorySize
// writeSenderString
// closeSenderMemory
// lockSenderMemory
// unlockSenderMemory
// createControl
// openControls
// checkControls
// closeControls
// openController

// RECEIVER
// setReceiverName
// createReceiver
// releaseReceiver
// receiveTexture
// senderDialog
// getSenderName
// getSenderWidth
// getSenderHeight

// COMMON
// getMemoryShareMode
// getTextureID

// FRAME COUNT
// getSenderFps
// getSenderFrame
// isFrameNew
// hasNewFrame
// disableFrameCount

// LOGGING
// enableSpoutLog
// spoutLogToFile
// spoutLogLevel
// spoutLogVerbose
// spoutLogNotice
// spoutLogWarning
// spoutLogError
// spoutLogFatal

// =================================================================== //
//                   INITIALIZE / DEINITIALIZE                         //
// =================================================================== //
JNIEXPORT jlong JNICALL Java_spout_JNISpout_init 
	(JNIEnv * env, jclass c)
{
 	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(c);

	jlong ptr = 0;
    
    Spout * spout = new Spout;
    
	ptr = (jlong)spout;
   
	return ptr;    
}

JNIEXPORT void JNICALL Java_spout_JNISpout_deInit 
	(JNIEnv * env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(c);

	Spout * spout = (Spout *)ptr;

	if(spout) {
		delete(spout);
	}

}


// =================================================================== //
//                             SENDER                                  //
// =================================================================== //

JNIEXPORT void JNICALL Java_spout_JNISpout_setSenderName
			(JNIEnv *env, jclass c, jstring name, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	char Sendername[256]; // user entered Sender name
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	try {
		const char *nativestring = env->GetStringUTFChars(name, &isCopy);

		// Set the sender name
		if (nativestring[0])
			strcpy_s(Sendername, 256, nativestring);
		else
			Sendername[0] = 0;

		spout->SetSenderName(Sendername);

		// release the input string
		env->ReleaseStringUTFChars(name, nativestring);

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in setSenderName", "JSpout", MB_OK);
	}
}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createSender
  (JNIEnv *env, jclass c, jstring name, jint width, jint height, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char Sendername[256]; // user entered Sender name
	unsigned int uWidth, uHeight;
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	try {
		const char *nativestring = env->GetStringUTFChars(name, &isCopy);

		// Set the sender name, width and height
		if(nativestring[0])
			strcpy_s(Sendername, 256, nativestring);
		else
			Sendername[0] = 0;

		uWidth = (unsigned int)width;
		uHeight = (unsigned int)height;

		//
		// Default settings are :
		//		DirectX 11
		//		Compatible texture format DXGI_FORMAT_B8G8R8A8_UNORM
		//

		// SPOUT CreateSender
		// name						- name of this sender
		// width, height			- width and height of this sender
		// dwFormat					- optional DX11 texture format
		//		DXGI_FORMAT_R8G8B8A8_UNORM - DX11 < > DX11
		//		DXGI_FORMAT_B8G8R8A8_UNORM - DX11 < > DX9 (default)
		// Also sender.SetDX9compatible(true / false); (true default)
		// Returns true for success or false for initialisation failure.
		bRet = spout->CreateSender(Sendername, uWidth, uHeight);

		// release the input string
		env->ReleaseStringUTFChars(name, nativestring);

		return bRet;

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in createSender", "JSpout", MB_OK);
		return false;
	}

}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_updateSender
	(JNIEnv *env, jclass c, jstring name, jint width, jint height, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char Sendername[256]; // user entered Sender name
	unsigned int uWidth, uHeight;
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	try {
		const char *nativestring = env->GetStringUTFChars(name, &isCopy);

		// Set the sender name, width and height
		if(nativestring[0])
			strcpy_s(Sendername, 256, nativestring);
		else
			Sendername[0] = 0;

		uWidth = (unsigned int)width;
		uHeight = (unsigned int)height;
	
		// Returns true for success or false for initialisation failure.
		bRet = spout->UpdateSender(Sendername, uWidth, uHeight);

		// release the input string
		env->ReleaseStringUTFChars(name, nativestring);

		return bRet;

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in updateSender", "JSpout", MB_OK);
		return false;
	}

}


JNIEXPORT jboolean JNICALL Java_spout_JNISpout_releaseSender 
	(JNIEnv *env, jclass c, jlong ptr) 
{

	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	spout->ReleaseSender();
	
	// Close the controls as well in case it is a controlled sender
	spoutcontrols.CloseControls();

	return true;

}


JNIEXPORT jboolean JNICALL Java_spout_JNISpout_sendTexture
	(JNIEnv *env, jclass c, jint width, jint height, jint texID, jint texTarget, jboolean bInvert, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	bool bInv;
	bool bRet = false;

	Spout * spout = (Spout *)ptr;

	// Do not quit if not initialized here for 2.007
	// because a 2.006 sender is created without a share handle

	// To quiet the compiler performance warning, use if/else rather than a cast
	if(bInvert) bInv = true;
	else bInv = false;

	GLint previousFBO = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &previousFBO);
	bRet = spout->SendTexture(texID, texTarget, width, height, bInv, previousFBO);

	return bRet;

}


// ======================================================= //
//                    Spout controls                       //
// ======================================================= //

//
// CreateControl(name, type, minimum, maximum, value, text)
//
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createControl
  (JNIEnv *env, jclass c, jstring name, jstring type, jfloat minimum, jfloat maximum, jfloat value, jstring text, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	bool bRet = false;
	char ControlName[256]; // control name
	char ControlType[256]; // control type
	char ControlText[256]; // control text
	float fMin;
	float fMax;
	float fValue;
	jboolean isCopy = JNI_FALSE;

	try {
		const char *nativestring1 = env->GetStringUTFChars(name, &isCopy);
		if(nativestring1[0])
			strcpy_s(ControlName, 256, nativestring1);
		else
			ControlName[0] = 0;

		const char *nativestring2 = env->GetStringUTFChars(type, &isCopy);
		if(nativestring2[0])
			strcpy_s(ControlType, 256, nativestring2);
		else
			ControlType[0] = 0;

		const char *nativestring3 = env->GetStringUTFChars(text, &isCopy);
		if(nativestring3[0])
			strcpy_s(ControlText, 256, nativestring3);
		else
			ControlText[0] = 0;

		fMin   = (float)minimum;
		fMax   = (float)maximum;
		fValue = (float)value;

		if(ControlName[0] && ControlType[0]) {

			// event
			// bool
			// float
			// text

			if(strcmp(ControlType, "event") == 0) // button
				bRet = spoutcontrols.CreateControl(ControlName, ControlType, fValue);
			else if(strcmp(ControlType, "bool") == 0) // checkbox
				bRet = spoutcontrols.CreateControl(ControlName, ControlType, fValue);
			else if(strcmp(ControlType, "text") == 0) {	// text
				if(ControlText[0])
					bRet = spoutcontrols.CreateControl(ControlName, ControlType, ControlText);
				else
					bRet = spoutcontrols.CreateControl(ControlName, ControlType);
			}
			else { // float
				bRet = spoutcontrols.CreateControl(ControlName, ControlType, fMin, fMax, fValue);
			}
		}

		// release the input strings
		env->ReleaseStringUTFChars(name, nativestring1);
		env->ReleaseStringUTFChars(name, nativestring2);
		env->ReleaseStringUTFChars(name, nativestring3);

		return (jboolean)bRet;

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in CreateControl(name, type, min, max, value, text)", "JSpout", MB_OK);
		return false;
	}

}



JNIEXPORT jboolean JNICALL Java_spout_JNISpout_openControls
	(JNIEnv *env, jclass c, jstring name, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	bool bRet = false;
	char Sendername[256]; // user entered Sender name
	jboolean isCopy = JNI_FALSE;

	try {
	
		const char *nativestring = env->GetStringUTFChars(name, &isCopy);

		// Set the sender name, width and height
		if(nativestring[0])
			strcpy_s(Sendername, 256, nativestring);
		else
			Sendername[0] = 0;

		bRet = spoutcontrols.OpenControls(Sendername);

		// release the input string
		env->ReleaseStringUTFChars(name, nativestring);

		return (jboolean)bRet;

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in OpenControls", "JSpout", MB_OK);
		return false;
	}

}

JNIEXPORT jint JNICALL Java_spout_JNISpout_checkControls 
	(JNIEnv * env, jclass c, jobjectArray controlName, jintArray controlType, jfloatArray controlValue, jobjectArray controlText, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	jint *Type = NULL;
	jfloat *Value = NULL;
	jboolean isCopy = JNI_FALSE;
	bool bText = false;
	bool bName = false;

	// Spout controls vector
	std::vector<control> Controls;

	if(spoutcontrols.CheckControls(Controls)) {

		if(Controls.size() > 0) {

			//
			// Initialization
			//

			// All the arrays are the same size
			// jsize stringCount = env->GetArrayLength(controlName);
			jsize stringCount = env->GetArrayLength(controlType);

			// Control Types
			Type = env->GetIntArrayElements(controlType, &isCopy);

			// Control Values
			Value = env->GetFloatArrayElements(controlValue, &isCopy);

			// Return names and text
			int mincount = min(stringCount, (int)Controls.size());
			for (int i=0; i<mincount; i++) {
			// for(int i=0; i<stringCount, i<(int)Controls.size(); i++) {
				// Return the names
				if(!Controls.at(i).name.empty()) {
					jstring string = env->NewStringUTF(Controls.at(i).name.c_str());
					env->SetObjectArrayElement(controlName, i, string);
					bName = true;
				}

				// Return the text
				if(!Controls.at(i).text.empty()) {
					jstring string = env->NewStringUTF(Controls.at(i).text.c_str());
					env->SetObjectArrayElement(controlText, i, string);
					bText = true;
				}

				// Return the type
				Type[i] = Controls.at(i).type;

				// Return the value
				Value[i] = Controls.at(i).value;

			}
		}

		env->ReleaseIntArrayElements(controlType, Type, 0);
		env->ReleaseFloatArrayElements(controlValue, Value, 0);

		if(bName)
			env->DeleteLocalRef(controlName);

		if(bText)
			env->DeleteLocalRef(controlText);

		env->DeleteLocalRef(controlType);
		env->DeleteLocalRef(controlValue);

		return (jint)Controls.size();
	}

	return 0;
}


JNIEXPORT jboolean JNICALL Java_spout_JNISpout_openController
	(JNIEnv *env, jclass c, jstring path, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	char SketchPath[MAX_PATH]; // user entered Sender name
	jboolean isCopy = JNI_FALSE;
	bool bRet = true;

	const char *nativestring = env->GetStringUTFChars(path, &isCopy);
	if(nativestring[0]) {
		strcpy_s(SketchPath, MAX_PATH, nativestring);
	}
	else {
		SketchPath[0] = 0;
	}

	HWND hwndControls = FindWindowA(NULL, (LPCSTR)"Spout Controller");
	if(!hwndControls) {
		// The controller is being started from within a dll so hide the "Close" button.
		// The Escape key when using the controller then hides it rather than closing it.
		// and the controller stays open until the sender controls are closed.
		if(SketchPath[0] == 0)
			bRet = spoutcontrols.OpenSpoutController("NoClose");
		else
			bRet = spoutcontrols.OpenSpoutController(SketchPath);
	}
	else {
		// Controller is already opened, so show or hide it
		if(hwndControls) {
			if(IsWindowVisible(hwndControls)) {
				// Hide it
				ShowWindow(hwndControls, SW_HIDE);
			}
			else {
				// Show it
				ShowWindow(hwndControls, SW_SHOW);
			}
		}
	}

	// release the input string
	env->ReleaseStringUTFChars(path, nativestring);

	return bRet;

}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_closeControls (JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	return(spoutcontrols.CloseControls());


}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_release
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	return(spoutcontrols.CloseControls());

}


// =======================================================
//      Shared memory for SpoutControls data transfer
//        Can only be used in texture share mode
//      May be removed dependent on SpoutControls
// =======================================================

// Create a shared memory map
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createSenderMemory
	(JNIEnv *env, jclass c, jstring name, jint width, jint height, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char Sendername[256]; // character array used by Spout functions
	unsigned int uWidth, uHeight;
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	// Set the sender name, width and height to local copy
	const char *nativestring = env->GetStringUTFChars(name, &isCopy);
	if(nativestring[0]) {
		strcpy_s(Sendername, 256, nativestring);
		uWidth  = (unsigned int)width;
		uHeight = (unsigned int)height;
		if(!spout->GetMemoryShareMode()) 
			bRet = memoryshare.CreateSenderMemory(Sendername, uWidth, uHeight);
	}
	env->ReleaseStringUTFChars(name, nativestring);

	return bRet;
}


// Update shared memory size - recreates the memory map
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_updateSenderMemorySize
	(JNIEnv *env, jclass c, jstring name, jint width, jint height, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char Sendername[256]; // character array used by Spout functions
	unsigned int uWidth, uHeight;
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	// Set the sender name, width and height to local copy
	const char *nativestring = env->GetStringUTFChars(name, &isCopy);
	if(nativestring[0]) {
		strcpy_s(Sendername, 256, nativestring);
		uWidth  = (unsigned int)width;
		uHeight = (unsigned int)height;
		if(!spout->GetMemoryShareMode()) 
			bRet = memoryshare.UpdateSenderMemorySize(Sendername, uWidth, uHeight);
	}
	env->ReleaseStringUTFChars(name, nativestring);

	return bRet;
}


// Write a string value (used by a sender)
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_writeSenderString
(JNIEnv *env, jclass c, jstring sValue, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	unsigned char *bufferout = NULL;
	unsigned int width = 0;
	unsigned int height = 0;
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	if (spout->GetMemoryShareMode()) return false;

	const char *nativestring = env->GetStringUTFChars(sValue, &isCopy);
	int nbytes = (int)strlen(nativestring);
	if (nativestring[0]) {

		// Check the shared memory size
		if (memoryshare.GetSenderMemorySize(width, height)) {
			int memsize = (int)(width*height * 4); // RGBA
			if (nbytes > memsize) nbytes = memsize;
			// Lock memory and retrieve buffer pointer to write to
			bufferout = memoryshare.LockSenderMemory();
			if (bufferout) {
				// Copy the bytes
				memcpy((void *)bufferout, (void *)nativestring, nbytes);
				memoryshare.UnlockSenderMemory();
				bRet = true;
			}
		}
	}

	env->ReleaseStringUTFChars(sValue, nativestring);

	return bRet;
}

// Close shared memory
JNIEXPORT void JNICALL Java_spout_JNISpout_closeSenderMemory
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	if(!spout->GetMemoryShareMode())
		memoryshare.CloseSenderMemory();

}


// Lock shared memory and return a pointer to the bytes
JNIEXPORT jlong JNICALL Java_spout_JNISpout_lockSenderMemory
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	unsigned char *pixels = NULL;

	Spout * spout = (Spout *)ptr;

	if(!spout->GetMemoryShareMode()) 
		pixels = memoryshare.LockSenderMemory();

	if(pixels)
		return (jlong)pixels;
	else
		return 0;

}

// Unlock memory to allow access
JNIEXPORT void JNICALL Java_spout_JNISpout_unlockSenderMemory
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	if(!spout->GetMemoryShareMode()) 
		memoryshare.UnlockSenderMemory();
}


// ======================================================= //
//                    Sync event functions                 //
// ======================================================= //

JNIEXPORT void JNICALL Java_spout_JNISpout_setFrameSync(JNIEnv *env, jclass c, jstring eventName, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	char name[256]; // character array used by Spout functions
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	// Set the event name to local copy
	const char *nativestring = env->GetStringUTFChars(eventName, &isCopy);
	if (nativestring[0]) {
		strcpy_s(name, 256, nativestring);
		spout->SetFrameSync(name);
	}
	env->ReleaseStringUTFChars(eventName, nativestring);
}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_waitFrameSync
(JNIEnv *env, jclass c, jstring eventName, jint timeout, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char name[256];
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	// Set the event name to local copy
	const char *nativestring = env->GetStringUTFChars(eventName, &isCopy);
	if (nativestring[0]) {
		strcpy_s(name, 256, nativestring);
		bRet = spout->WaitFrameSync(name, (DWORD)timeout);
	}
	env->ReleaseStringUTFChars(eventName, nativestring);

	return bRet;
}


// ======================================================= //
//           Shared memory data exchange functions         //
// ======================================================= //

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_writeMemoryBuffer
(JNIEnv *env, jclass c, jstring bufferName, jstring buffer, jint length, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char name[256];
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	const char *nativename = env->GetStringUTFChars(bufferName, &isCopy);
	if (nativename[0]) {
		strcpy_s(name, 256, nativename);
		// Set the buffer string to local char copy
		const char *nativedata = env->GetStringUTFChars(buffer, &isCopy);
		// Write the char array
		if (nativedata[0])
			bRet = spout->WriteMemoryBuffer(name, nativedata, (DWORD)length);
		env->ReleaseStringUTFChars(buffer, nativedata);
	}
	env->ReleaseStringUTFChars(bufferName, nativename);
	return bRet;
}


JNIEXPORT jstring JNICALL Java_spout_JNISpout_readMemoryBuffer
(JNIEnv *env, jclass c, jstring bufferName, jint maxlength, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	char name[256];
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	// Create a receiving char array
	char *senderbuffer = new char[maxlength];

	// Clear buffer for return if ReadMemoryBuffer is not successful
	senderbuffer[0] = 0;

	const char *nativename = env->GetStringUTFChars(bufferName, &isCopy);
	if (nativename[0]) {
		strcpy_s(name, 256, nativename);
		spout->ReadMemoryBuffer(name, senderbuffer, maxlength);
	}

	// Create a string from the char array
	jstring jstrSenderBuffer = env->NewStringUTF(senderbuffer);
	
	// Done with the char array
	delete[] senderbuffer;
	
	env->ReleaseStringUTFChars(bufferName, nativename);

	return jstrSenderBuffer;
}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_createMemoryBuffer
(JNIEnv *env, jclass c, jstring bufferName, jint length, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	char name[256];
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	const char *nativename = env->GetStringUTFChars(bufferName, &isCopy);
	if (nativename[0]) {
		strcpy_s(name, 256, nativename);
		bRet = spout->CreateMemoryBuffer(name, (DWORD)length);
	}
	env->ReleaseStringUTFChars(bufferName, nativename);

	return bRet;
}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_deleteMemoryBuffer
(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;
	return spout->DeleteMemoryBuffer();

}

JNIEXPORT jint JNICALL Java_spout_JNISpout_getMemoryBufferSize
(JNIEnv *env, jclass c, jstring bufferName, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	char name[256];
	int iRet = 0;
	jboolean isCopy = JNI_FALSE;
	Spout * spout = (Spout *)ptr;

	const char *nativename = env->GetStringUTFChars(bufferName, &isCopy);
	if (nativename[0]) {
		strcpy_s(name, 256, nativename);
		iRet = spout->GetMemoryBufferSize(name);
	}
	env->ReleaseStringUTFChars(bufferName, nativename);

	return iRet;

}


// =================================================================== //
//                            RECEIVER                                 //
// =================================================================== //

JNIEXPORT void JNICALL Java_spout_JNISpout_setReceiverName
				(JNIEnv *env, jclass c, jstring name, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	char Sendername[256]; // user entered Sender name
	jboolean isCopy = JNI_FALSE;

	Spout * spout = (Spout *)ptr;

	try {
		const char *nativestring = env->GetStringUTFChars(name, &isCopy);

		// Set the receiver name
		if (nativestring[0])
			strcpy_s(Sendername, 256, nativestring);
		else
			Sendername[0] = 0;

		spout->SetReceiverName(Sendername);

		// release the input string
		env->ReleaseStringUTFChars(name, nativestring);

	}
	catch (...) {
		MessageBoxA(NULL, "Exception in setReceiverName", "JSpout", MB_OK);
	}
}

JNIEXPORT boolean JNICALL Java_spout_JNISpout_createReceiver
	(JNIEnv *env, jclass c, jstring name, jintArray dimarray, jlong ptr) 
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	unsigned int uWidth, uHeight;
	char Sendername[256]; // user entered Sender name

	Spout * spout = (Spout *)ptr;

	jboolean isCopy = JNI_FALSE;
	jint *dim = env->GetIntArrayElements(dimarray, &isCopy);
	isCopy = JNI_FALSE;

	uWidth  = (unsigned int)dim[0];	// whatever was passed
	uHeight = (unsigned int)dim[1];

	const char *nativestring = env->GetStringUTFChars(name, &isCopy);
	if(nativestring[0])
		strcpy_s(Sendername, 256, nativestring);
	else
		Sendername[0] = 0;

	//
	// Default settings are	DirectX 11
	//

	//
	// SPOUT createReceiver
	//
	// Returns :
	// name						- name of the sender found if the name passed was null
	// width, height			- width and height of the sender
	// Returns true for success or false for initialisation failure.

	bRet = spout->CreateReceiver(Sendername, uWidth, uHeight);

	if(bRet) {
		// Pass back the width and height of the sender it connected to
		// 2.007
		// Don't pass back the sender name
		// the new name can be retreived with GetSenderName
		dim[0] = (jint)uWidth;
		dim[1] = (jint)uHeight;
	}

	// JNI_ABORT - done with the data which is then freed and not passed back
	// JNI_COMMIT - done with the data which is passed back but not freed
	// 0 - done with the data which is passed back and freed
	env->ReleaseIntArrayElements(dimarray, dim, 0);
	env->DeleteLocalRef(dimarray);
	env->ReleaseStringUTFChars(name, nativestring);

	return bRet;

}

JNIEXPORT jboolean JNICALL Java_spout_JNISpout_releaseReceiver (JNIEnv *env, jclass c, jlong ptr) {

	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	spout->ReleaseReceiver();

	return true;

}


// COPY THE SHARED TEXTURE TO A TEXTURE
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_receiveTexture
	(JNIEnv *env, jclass c, jintArray dimarray, jint texName, jint texTarget, jboolean bInvert, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	bool bRet = false;
	bool bInv = false;
	jboolean isCopy = FALSE;
	unsigned int width = 0;
	unsigned int height = 0;
	char sendername[256];

	Spout * spout = (Spout *)ptr;
		
	if (!spout->IsSpoutInitialized()) {
		return false;
	}

	if(bInvert) bInv = true;
	else bInv = false;

	isCopy = JNI_FALSE;
	jint *dim = env->GetIntArrayElements(dimarray, &isCopy);

	width  = (unsigned int)dim[0];
	height = (unsigned int)dim[1];

	if(width > 0 && height > 0) {

		// retrieve the current fbo
		GLint previousFBO = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &previousFBO);

		// Retrieve the shared texture into the passed texture
		if (spout->GetSenderName()) {

			strcpy_s(sendername, 256, spout->GetSenderName());

			if (spout->ReceiveTexture(sendername, width, height, texName, texTarget, bInv, previousFBO)) {
				// Return the dimensions in case the received texture has changed
				// The application then manages the size change
				dim[0] = (jint)width;
				dim[1] = (jint)height;
				bRet = true;
			}
		}
	}

	env->ReleaseIntArrayElements(dimarray, dim, 0);
	env->DeleteLocalRef(dimarray);
	
	return bRet;

}

// COPY THE SHARED TEXTURE TO IMAGE PIXELS
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_receivePixels (JNIEnv *env, jclass c, jintArray dimarray, jintArray pixarray, jlong ptr)
{
	bool bRet = false;

	jboolean isCopy = FALSE;
	unsigned int width = 0;
	unsigned int height = 0;
	char sendername[256];

	UNREFERENCED_PARAMETER(c);

	// Quit if not initialized
	Spout * spout = (Spout *)ptr;
	if(!spout->IsSpoutInitialized()) return false;

	// should check isCopy here
	isCopy = JNI_FALSE;
	jint *dim = env->GetIntArrayElements(dimarray, &isCopy);

	isCopy = JNI_FALSE;
	jint *pix = env->GetIntArrayElements(pixarray, &isCopy);
	
	width  = (unsigned int)dim[0];
	height = (unsigned int)dim[1];

	if(width > 0 && height > 0) {

		// Retrieve opengl texture data directly to image pixels
		// bytes need to be reversed. Transfer directly using BGRA
		// PBO used if the extensions are available
		// Invert not implemented but required for latest 2.005 SDK function
		if (spout->GetSenderName()) {
			strcpy_s(sendername, 256, spout->GetSenderName());
			GLint previousFBO;
			glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &previousFBO);
			if(spout->ReceiveImage(sendername, width, height, (unsigned char *)pix, GL_BGRA_EXT, false, previousFBO)) {
				bRet = true;
				dim[0] = (jint)width;
				dim[1] = (jint)height;
			}
		}
	}

	// JNI_ABORT - done with the data which is then freed and not passed back
	// JNI_COMMIT - done with the data which is passed back but not freed
	// 0 - done with the data which is passed back and freed
	env->ReleaseIntArrayElements(dimarray, dim, 0);
	env->DeleteLocalRef(dimarray);
	env->ReleaseIntArrayElements(pixarray, pix, 0);
	env->DeleteLocalRef(pixarray);

	return bRet;

}


// Return a selected Sender name from a dialog showing the list in shared memory
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_senderDialog
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	if(spout->GetDX9()) {
		return(spout->SelectSenderPanel("/DX9"));
	}
	else {
		return(spout->SelectSenderPanel("/DX11"));
	}

}

// TODO : simplify ?
// Get the current global sender name
JNIEXPORT jstring JNICALL Java_spout_JNISpout_getSenderName
(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);

	char name[256];
	char Sendername[256];
	unsigned int width, height;
	DWORD dwFormat;
	HANDLE dxShareHandle;

	Spout * spout = (Spout *)ptr;

	Sendername[0] = 0;

	if (spout->GetSenderCount() > 0) {
		if (spout->GetSenderName()) {
			strcpy_s(name, 256, spout->GetSenderName());
			// Does it still exist ?
			if (spout->GetSenderInfo(name, width, height, dxShareHandle, dwFormat))
				strcpy_s(Sendername, 256, name);
		}
	}

	jstring jstrSenderName = env->NewStringUTF(Sendername);

	return jstrSenderName;
}

// Get the current sender width
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderWidth(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	unsigned int width = 0;
	// unsigned int height = 0;
	// DWORD dwFormat = 0;
	// HANDLE dxShareHandle = 0;
	Spout * spout = (Spout *)ptr;
	if (spout->GetSenderCount() > 0) {
		width = spout->GetSenderWidth();
	}
	return width;
}

// Get the current sender height
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderHeight(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	// unsigned int width = 0;
	unsigned int height = 0;
	// DWORD dwFormat = 0;
	// HANDLE dxShareHandle = 0;
	Spout * spout = (Spout *)ptr;
	if (spout->GetSenderCount() > 0) {
		height = spout->GetSenderHeight();
	}
	return height;
}

// Return Sender fps
JNIEXPORT jfloat JNICALL Java_spout_JNISpout_getSenderFps(JNIEnv * env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	return((float)spout->GetSenderFps());

}

// Get the current sender frame number
JNIEXPORT jint JNICALL Java_spout_JNISpout_getSenderFrame(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;
	return (jint)spout->GetSenderFrame();
}

// Is the received frame new
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_isFrameNew
(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	return spout->IsFrameNew();
}

// Disable frame counting
JNIEXPORT void JNICALL Java_spout_JNISpout_disableFrameCount(JNIEnv * env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;

	spout->DisableFrameCount();

}

// =================================================================== //
//                             UTILITY                                 //
// =================================================================== //

// Get the memorysharing mode
JNIEXPORT jboolean JNICALL Java_spout_JNISpout_getMemoryShareMode
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;
	return spout->GetMemoryShareMode();
}

// Get the sharing mode
// 0 - texture, 1 - CPU, 2 - memory
JNIEXPORT jint JNICALL Java_spout_JNISpout_getShareMode
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;
	return spout->GetShareMode();
}

// Get the shared OpenGL texture ID
JNIEXPORT jint JNICALL Java_spout_JNISpout_getTextureID
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);

	Spout * spout = (Spout *)ptr;
	return spout->GetSharedTextureID();

}

//
// 2.007 logging
//

// Enable logging to the console
JNIEXPORT void JNICALL Java_spout_JNISpout_enableSpoutLog
	(JNIEnv *env, jclass c, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	// SpoutUtils namespace
	EnableSpoutLog();
}


// Enable logging to a file with optional append
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogToFile
(JNIEnv *env, jclass c, jstring logfile, jboolean append, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogFile[MAX_PATH]; // user entered log file

	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logfile, &isCopy);
	if (nativestring[0])
		strcpy_s(LogFile, MAX_PATH, nativestring);
	else
		LogFile[0] = 0;

	// SpoutUtils namespace
	EnableSpoutLogFile(LogFile, append);

	env->ReleaseStringUTFChars(logfile, nativestring);
}

// Set the current log level
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogLevel
	(JNIEnv *env, jclass c, jint level, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(env);
	UNREFERENCED_PARAMETER(ptr);

	// SpoutUtils namespace
	SetSpoutLogLevel((SpoutLogLevel)level);
}

// spoutLog
// spoutLogVerbose
// spoutLogNotice
// spoutLogWarning
// spoutLogError
// spoutLogFatal
JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLog
(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;

	// SpoutUtils namespace
	SpoutLog(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}

JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogVerbose
	(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;
	
	// SpoutUtils namespace
	SpoutLogVerbose(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}

JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogNotice
	(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;

	// SpoutUtils namespace
	SpoutLogNotice(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}

JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogWarning
	(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;

	// SpoutUtils namespace
	SpoutLogWarning(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}

JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogError
	(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;

	// SpoutUtils namespace
	SpoutLogError(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}

JNIEXPORT void JNICALL Java_spout_JNISpout_spoutLogFatal
	(JNIEnv *env, jclass c, jstring logtext, jlong ptr)
{
	UNREFERENCED_PARAMETER(c);
	UNREFERENCED_PARAMETER(ptr);

	char LogText[MAX_PATH]; // user entered log text
	jboolean isCopy = JNI_FALSE;
	const char *nativestring = env->GetStringUTFChars(logtext, &isCopy);
	if (nativestring[0])
		strcpy_s(LogText, MAX_PATH, nativestring);
	else
		LogText[0] = 0;

	// SpoutUtils namespace
	SpoutLogFatal(LogText);

	env->ReleaseStringUTFChars(logtext, nativestring);
}



// =================================================================== //
//                             LOCAL                                   //
// =================================================================== //

void SaveOpenGLstate()
{
	// save texture state, client state, etc.
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_TRANSFORM_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity(); // reset the current matrix back to its default state
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}


void RestoreOpenGLstate()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
		
	glPopAttrib();
		
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();

	glPopClientAttrib();			
	glPopAttrib();

}


// GL_INVALID_ENUM                   0x0500	1280
// Given when an enumeration parameter is not a legal enumeration for that function.
// This is given only for local problems; if the spec allows the enumeration in certain circumstances,
// where other parameters or state dictate those circumstances, then GL_INVALID_OPERATION is the result instead.

// GL_INVALID_VALUE                  0x0501	1281

// GL_INVALID_OPERATION              0x0502	1282
// GL_STACK_OVERFLOW                 0x0503	1283
// GL_STACK_UNDERFLOW                0x0504	1284
// GL_OUT_OF_MEMORY                  0x0505 1285
// GL_INVALID_FRAMEBUFFER_OPERATION  0x0506 1286

void CheckGLerror()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
		std::string error;
        switch(err) {
			case GL_INVALID_OPERATION: error="INVALID_OPERATION"; break;
            case GL_INVALID_ENUM:      error="INVALID_ENUM";      break;
            case GL_INVALID_VALUE:     error="INVALID_VALUE";     break;
            case GL_OUT_OF_MEMORY:     error="OUT_OF_MEMORY";     break;
            // case GL_INVALID_FRAMEBUFFER_OPERATION:  error="INVALID_FRAMEBUFFER_OPERATION";  break;
        }
		// print the error
    }
}



