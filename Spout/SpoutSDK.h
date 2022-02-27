/*

					SpoutSDK.h

	Copyright (c) 2014-2021, Lynn Jarvis. All rights reserved.

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

*/
#pragma once

#ifndef __SpoutSDK__
#define __SpoutSDK__

#include "../../SpoutGL.h"
#include "SpoutMemoryShare.h"

class SPOUT_DLLEXP SpoutSDK : public spoutGL {

	public:

	SpoutSDK();
    ~SpoutSDK();

	//
	// ===================== SENDER =========================
	//

	// Set name for sender creation
	//   If no name is specified, the executable name is used.  
	void SetSenderName(const char* sendername = nullptr);
	// Set the sender DX11 shared texture format
	void SetSenderFormat(DWORD dwFormat);
	// Close sender and free resources
	//   A sender is created or updated by all sending functions
	void ReleaseSender();
	// Send texture attached to fbo.
	//   The fbo must be currently bound.  
	//   The sending texture can be larger than the size that the sender is set up for.  
	//   For example, if the application is using only a portion of the allocated texture space,  
	//   such as for Freeframe plugins. (The 2.006 equivalent is DrawToSharedTexture).
	bool SendFbo(GLuint FboID, unsigned int width, unsigned int height, bool bInvert = true);
	// Send OpenGL texture
	bool SendTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, bool bInvert = true, GLuint HostFBO = 0);
	// Send image pixels
	bool SendImage(const unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFBO = 0);
	// Sender status
	bool IsInitialized();
	// Sender name
	const char * GetName();
	// Sender width
	unsigned int GetWidth();
	// Sender height
	unsigned int GetHeight();
	// Sender frame rate
	double GetFps();
	// Sender frame number
	long GetFrame();
	// Sender share handle
	HANDLE GetHandle();
	// Sender frame rate control
	void HoldFps(int fps);

	//
	// 2.006 compatibility
	//

	// Create a sender
	bool CreateSender(const char *Sendername, unsigned int width = 0, unsigned int height = 0, DWORD dwFormat = 0);
	// Update a sender
	bool UpdateSender(const char* Sendername, unsigned int width, unsigned int height);

	// Legacy OpenGL DrawTo function
	// See _SpoutCommon.h_ #define legacyOpenGL
#ifdef legacyOpenGL
	// Render a texture to the shared texture. 
	bool DrawToSharedTexture(GLuint TextureID, GLuint TextureTarget, unsigned int width, unsigned int height, float max_x = 1.0, float max_y = 1.0, float aspect = 1.0, bool bInvert = false, GLuint HostFBO = 0);
#endif

	// Sender creation and change
	bool CheckSender(unsigned int width, unsigned int height);

	spoutMemoryShare memoryshare;

	//
	// ====================== RECEIVER ===========================
	//

		// Specify sender for connection
	//   The application will not connect to any other  unless the user selects one
	//   If that sender closes, the application will wait for the nominated sender to open 
	//   If no name is specified, the receiver will connect to the active sender
	void SetReceiverName(const char * sendername);
	// Close receiver and release resources ready to connect to another sender
	void ReleaseReceiver();
	// Receive shared texture
	//   Connect to a sender and retrieve texture details ready for access
	//	 (see BindSharedTexture and UnBindSharedTexture)
	bool ReceiveTexture();
	// Receive OpenGL texture
	// 	 Connect to a sender and inform the application to update
	//   the receiving texture if it has changed dimensions
	//   For no change, copy the sender shared texture to the application texture
	bool ReceiveTexture(GLuint TextureID, GLuint TextureTarget, bool bInvert = false, GLuint HostFbo = 0);
	// Receive image pixels
	//   Connect to a sender and inform the application to update
	//   the receiving buffer if it has changed dimensions
	//   For no change, copy the sender shared texture to the pixel buffer
	bool ReceiveImage(unsigned char* pixels, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFbo = 0);
	// Query whether the sender has changed
	//   Checked at every cycle before receiving data
	bool IsUpdated();
	// Query sender connection
	//   If the sender closes, receiving functions return false,  
	bool IsConnected();
	// Query received frame status
	//   The receiving texture or pixel buffer is only refreshed if the sender has produced a new frame  
	//   This can be queried to process texture data only for new frames
	bool IsFrameNew();
	// Received sender name
	const char * GetSenderName();
	// Received sender width
	unsigned int GetSenderWidth();
	// Received sender height
	unsigned int GetSenderHeight();
	// Received sender DX11 texture format
	DWORD GetSenderFormat();
	// Received sender frame rate
	double GetSenderFps();
	// Received sender frame number
	long GetSenderFrame();
	// Received sender share handle
	HANDLE GetSenderHandle();
	// Received sender sharing mode
	bool GetSenderCPUmode();
	// Open sender selection dialog
	void SelectSender();

	//
	// 2.006 compatibility
	//

	// Create receiver connection
	bool CreateReceiver(char* Sendername, unsigned int &width, unsigned int &height, bool bUseActive = false);
	// Check receiver connection
	bool CheckReceiver(char* Sendername, unsigned int &width, unsigned int &height, bool &bConnected);
	// Receive OpenGL texture
	bool ReceiveTexture(char* Sendername, unsigned int &width, unsigned int &height, GLuint TextureID = 0, GLuint TextureTarget = 0, bool bInvert = false, GLuint HostFBO = 0);
	// Receive image pixels
	bool ReceiveImage(char* Sendername, unsigned int &width, unsigned int &height, unsigned char* pixels, GLenum glFormat = GL_RGBA, bool bInvert = false, GLuint HostFBO = 0);
	// Open dialog for the user to select a sender
	//   Optional message argument
	bool SelectSenderPanel(const char* message = nullptr);

	// Legacy OpenGL Draw function
	// See _SpoutCommon.h_ #define legacyOpenGL
#ifdef legacyOpenGL
	// Render the shared texture
	bool DrawSharedTexture(float max_x = 1.0, float max_y = 1.0, float aspect = 1.0, bool bInvert = true, GLuint HostFBO = 0);
#endif

protected:

	// Create receiver connection
	void InitReceiver(const char * sendername, unsigned int width, unsigned int height, DWORD dwFormat);
	// Receiver find sender and retrieve information
	bool ReceiveSenderData();
	// Receiver detect sender selection
	bool CheckSpoutPanel(char *sendername, int maxchars = 256);


};

#endif
