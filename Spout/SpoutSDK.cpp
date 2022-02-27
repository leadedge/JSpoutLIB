//
//		SpoutSDK
//
// ====================================================================================
//		Revisions :
//
//
// ====================================================================================
/*

	Copyright (c) 2021, Lynn Jarvis. All rights reserved.

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
#include "SpoutSDK.h"

SpoutSDK::SpoutSDK()
{

}

SpoutSDK::~SpoutSDK()
{
	
}

//
//==================== SENDER =============================
//

//---------------------------------------------------------
void SpoutSDK::SetSenderName(const char* sendername)
{
	if (!sendername) {
		// Get executable name as default
		GetModuleFileNameA(NULL, m_SenderName, 256);
		PathStripPathA(m_SenderName);
		PathRemoveExtensionA(m_SenderName);
	}
	else {
		strcpy_s(m_SenderName, 256, sendername);
	}
}

//---------------------------------------------------------
void SpoutSDK::SetSenderFormat(DWORD dwFormat)
{
	m_dwFormat = dwFormat;
	m_DX11format = (DXGI_FORMAT)dwFormat;
}

//---------------------------------------------------------
void SpoutSDK::ReleaseSender()
{
	if (m_bInitialized) {
		sendernames.ReleaseSenderName(m_SenderName);
		frame.CleanupFrameCount();
		frame.CloseAccessMutex();
	}

	CleanupGL();

}

//---------------------------------------------------------
bool SpoutSDK::SendFbo(GLuint FboID, unsigned int width, unsigned int height, bool bInvert)
{
	// For texture sharing, the fbo must be equal to or larger than the shared texture
	if (FboID <= 0 || width == 0 || height == 0)
		return false;

	// Create or update the sender
	if (!CheckSender(width, height))
		return false;

	// All clear to send the fbo texture
	if (m_bUseGLDX) {
		// 3840-2160 - 60fps (0.45 msec per frame)
		return WriteGLDXtexture(0, 0, width, height, bInvert, FboID);
	}
	else if (m_bAuto) {
		// DirectX 11 CPU backup mode
		// 3840-2160 - 43fps (5-7msec/frame)
		// Create a local class texture if not already
		CheckOpenGLTexture(m_TexID, GL_RGBA, width, height);
		// Copy from the texture attached to the bound fbo to the class texture
		glBindTexture(GL_TEXTURE_2D, m_TexID);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, width, height);
		glBindTexture(GL_TEXTURE_2D, 0);
		// Copy from the OpenGL class texture to the shared DX11 texture by way of staging texture
		return WriteDX11texture(m_TexID, GL_TEXTURE_2D, width, height, bInvert, 0);
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::SendTexture(GLuint TextureID, GLuint TextureTarget,
	unsigned int width, unsigned int height, bool bInvert, GLuint HostFBO)
{
	// Quit if no data
	if (TextureID <= 0 || width == 0 || height == 0)
		return false;

	// Create or update the sender
	if (!CheckSender(width, height))
		return false;

	if (m_bUseGLDX) {
		// Send OpenGL texture if GL/DX interop compatible
		// 3840-2160 - 60fps (0.45 msec per frame)
		return WriteGLDXtexture(TextureID, TextureTarget, width, height, bInvert, HostFBO);
	}
	else if (m_bAuto) {
		// CPU backup mode DirectX 11
		// 3840-2160 47fps (6-7 msec per frame with PBOs)
		return WriteDX11texture(TextureID, TextureTarget, width, height, bInvert, HostFBO);
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::SendImage(const unsigned char* pixels, unsigned int width, unsigned int height, GLenum glFormat, bool bInvert, GLuint HostFBO)
{

	// Dimensions should be the same as the sender
	if (!pixels || width == 0 || height == 0)
		return false;

	// Only RGBA, BGRA, RGB, BGR supported
	if (!(glFormat == GL_RGBA || glFormat == GL_BGRA_EXT || glFormat == GL_RGB || glFormat == GL_BGR_EXT))
		return false;

	// Check for BGRA support
	GLenum glformat = glFormat;
	if (!m_bBGRAavailable) {
		// If the bgra extensions are not available and the user
		// provided GL_BGR_EXT or GL_BGRA_EXT do not use them
		if (glFormat == GL_BGR_EXT) glformat = GL_RGB;
		if (glFormat == GL_BGRA_EXT) glformat = GL_RGBA;
	}

	// Create or update the sender
	if (!CheckSender(width, height))
		return false;

	//
	// Write pixel data to the rgba shared texture according to pixel format
	//

	if (m_bUseGLDX) {
		return WriteGLDXpixels(pixels, width, height, glformat, bInvert, HostFBO);
	}
	else if (m_bAuto) {
		return WriteDX11pixels(pixels, width, height, glformat, bInvert);
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::IsInitialized()
{
	return m_bInitialized;
}

//---------------------------------------------------------
const char * SpoutSDK::GetName()
{
	return m_SenderName;
}

//---------------------------------------------------------
unsigned int SpoutSDK::GetWidth()
{
	return m_Width;
}

//---------------------------------------------------------
unsigned int SpoutSDK::GetHeight()
{
	return m_Height;
}

//---------------------------------------------------------
double SpoutSDK::GetFps()
{
	return frame.GetSenderFps();
}

//---------------------------------------------------------
long SpoutSDK::GetFrame()
{
	return frame.GetSenderFrame();
}

//---------------------------------------------------------
HANDLE SpoutSDK::GetHandle()
{
	return m_dxShareHandle;
}

//---------------------------------------------------------
// Controls the frame rate if no other means is available
void SpoutSDK::HoldFps(int fps)
{
	frame.HoldFps(fps);
}


//
// 2.006 compatibility
//

//---------------------------------------------------------
bool SpoutSDK::CreateSender(const char* name, unsigned int width, unsigned int height, DWORD dwFormat)
{
	if (width == 0 || height == 0) {
		SetSenderName(name);
	}
	else if (sendernames.CreateSender(name, width, height, m_dxShareHandle, dwFormat)) {

		strcpy_s(m_SenderName, 256, name);
		m_Width = width;
		m_Height = height;
		if (dwFormat > 0)
			m_dwFormat = dwFormat;

		// Set CPU sharing mode (i.e. not GL/DX compatible)
		// to the top bit of 32 bit partnerID field in sender shared memory
		SetSenderCPUmode(m_SenderName, !m_bUseGLDX);

		// Write host path to the sender shared memory Description field
		SetHostPath(m_SenderName);

		// Get current adapter index and name
		m_AdapterIndex = GetAdapter();
		GetAdapterName(m_AdapterIndex, m_AdapterName, 256);

		// Do not set initialized here for 2.007
		// because a 2.006 sender can be created without a share handle in Processing
		// the sharehandle is subsequently created and used in CheckSender

		return true;
	}
	else {
		m_SenderName[0] = 0;
		m_Width = 0;
		m_Height = 0;
		m_dwFormat = m_DX11format;
		m_AdapterIndex = 0;
		m_AdapterName[0] = 0;
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::UpdateSender(const char* name, unsigned int width, unsigned int height)
{
	// No update unless already created
	if (!IsInitialized())
		return false;

	bool bRet = false;
	// For a name change, close the sender and set up again
	if (strcmp(name, m_SenderName) != 0) {
		ReleaseSender();
		// CreateSender sets m_Width and m_Height on success
		bRet = CreateSender(name, width, height, m_dwFormat);
	}
	else if (width != m_Width || height != m_Height) {
		// For sender update, only width and height are necessary
		if (sendernames.UpdateSender(m_SenderName, width, height, m_dxShareHandle)) {
			m_Width = width;
			m_Height = height;
			bRet = true;
		}
	}
	return bRet;
}

// Legacy OpenGL DrawTo function
#ifdef legacyOpenGL

//---------------------------------------------------------
bool SpoutSDK::DrawToSharedTexture(GLuint TextureID, GLuint TextureTarget,
	unsigned int width, unsigned int height,
	float max_x, float max_y, float aspect,
	bool bInvert, GLuint HostFBO)
{
	GLenum status;
	bool bRet = false;

	if (!m_hInteropDevice || !m_hInteropObject)
		return false;

	if (width != (unsigned  int)m_Width || height != (unsigned  int)m_Height)
		return false;

	// Wait for access to the shared texture
	if (frame.CheckTextureAccess(m_pSharedTexture)) {
		if (LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			// Draw the input texture into the shared texture via an fbo
			// Bind our fbo and attach the shared texture to it
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			glFramebufferTexture2DEXT(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_glTexture, 0);
			status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
			if (status == GL_FRAMEBUFFER_COMPLETE_EXT) {
				glColor4f(1.f, 1.f, 1.f, 1.f);
				glEnable(TextureTarget);
				glBindTexture(TextureTarget, TextureID);
				GLfloat tc[4][2] = { 0 };
				// Invert texture coord to user requirements
				if (bInvert) {
					tc[0][0] = 0.0;   tc[0][1] = max_y;
					tc[1][0] = 0.0;   tc[1][1] = 0.0;
					tc[2][0] = max_x; tc[2][1] = 0.0;
					tc[3][0] = max_x; tc[3][1] = max_y;
				}
				else {
					tc[0][0] = 0.0;   tc[0][1] = 0.0;
					tc[1][0] = 0.0;   tc[1][1] = max_y;
					tc[2][0] = max_x; tc[2][1] = max_y;
					tc[3][0] = max_x; tc[3][1] = 0.0;
				}
				GLfloat verts[] = {
								-aspect, -1.0,   // bottom left
								-aspect,  1.0,   // top left
								 aspect,  1.0,   // top right
								 aspect, -1.0 }; // bottom right
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(2, GL_FLOAT, 0, tc);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(2, GL_FLOAT, 0, verts);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindTexture(TextureTarget, 0);
				glDisable(TextureTarget);
				bRet = true; // success
			}
			else {
				PrintFBOstatus(status);
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);
				UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
			}
			// restore the previous fbo - default is 0
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, HostFBO);
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject);
		} // end interop lock
		// Release mutex and allow access to the texture
		frame.AllowTextureAccess(m_pSharedTexture);
	} // mutex access failed

	return bRet;

} // end DrawToSharedTexture
#endif


//
// Protected functions
//

//---------------------------------------------------------
// If a sender has not been created yet
//    o Make sure Spout has been initialized and OpenGL context is available
//    o Perform a compatibility check for GL/DX interop
//    o If compatible, create interop for GL/DX transfer
//    o If not compatible, create a shared texture for the sender
//    o Create a sender using the DX11 shared texture handle
// If the sender exists, test for size change
//    o If compatible, update the shared textures and GL/DX interop
//    o If not compatible, re-create the class shared texture to the new size
//    o Update the sender and class variables	
bool SpoutSDK::CheckSender(unsigned int width, unsigned int height)
{
	if (width == 0 || height == 0)
		return false;

	// The sender needs a name
	// Default is the executable name
	if (!m_SenderName[0])
		SetSenderName();

	if (!m_bInitialized) {

		// Make sure Spout has been initialized and OpenGL context is available
		// The DX11 graphics adapter could be different, so re-test compatibility for GL/DX
		if (OpenSpout(true)) {

			if (m_bUseGLDX) {
				// Create interop for GL/DX transfer
				// Flag "false" for sender so that a new shared texture is created
				// (for a receiver the shared texture is cretaed from the sender share handle)
				if (!CreateInterop(width, height, m_dwFormat, false))
					return false;
			}
			else {
				
				// 2.006 compatibility check
				// CPU fall-back only for DX11 textures
				if (GetDX9())
					return false;

				SpoutLogWarning("SpoutSDK::CheckSender - Graphics not GL/DX compatible.");

				// Create a shared texture for the sender within this class.
				// A sender creates a new texture with a new share handle
				m_dxShareHandle = nullptr;
				if (!spoutdx.CreateSharedDX11Texture(spoutdx.GetDX11Device(),
					width, height, (DXGI_FORMAT)m_dwFormat, &m_pSharedTexture, m_dxShareHandle)) {
					return false;
				}

			}

			// Create a sender using the DX11 shared texture handle (m_dxShareHandle)
			m_bInitialized = CreateSender(m_SenderName, width, height, m_dwFormat);
			m_Width = width;
			m_Height = height;

			// Create a sender mutex for access to the shared texture
			frame.CreateAccessMutex(m_SenderName);

			// Enable frame counting so the receiver gets frame number and fps
			frame.EnableFrameCount(m_SenderName);
		}
	}
	// The sender is initialized but has the sending texture changed size ?
	else if (m_Width != width || m_Height != height) {

		// Update the shared textures and interop
		if (m_bUseGLDX) {
			// Flag "false" for sender to create a new shared texture
			if (!CreateInterop(width, height, m_dwFormat, false))
				return false;
		}
		else {
			// CPU fall-back only for DX11 textures
			if (GetDX9())
				return false;
			// Re-create the class shared texture to the new size
			if (m_pSharedTexture) m_pSharedTexture->Release();
			m_pSharedTexture = nullptr;
			m_dxShareHandle = nullptr;
			if (!spoutdx.CreateSharedDX11Texture(spoutdx.GetDX11Device(),
				width, height, (DXGI_FORMAT)m_dwFormat, &m_pSharedTexture, m_dxShareHandle)) {
				return false;
			}
		}

		// Update the sender and class variables
		sendernames.UpdateSender(m_SenderName, width, height, m_dxShareHandle, m_dwFormat);
		m_Width = width;
		m_Height = height;

	} // endif initialization or size checks

	return true;
}


//
// ===================== RECEIVER =========================
//

//---------------------------------------------------------
// Specify the sender for a receiver to connect to
void SpoutSDK::SetReceiverName(const char * SenderName)
{
	if (SenderName && SenderName[0]) {
		strcpy_s(m_SenderNameSetup, 256, SenderName);
		strcpy_s(m_SenderName, 256, SenderName);
	}
}


//---------------------------------------------------------
// Release receiver and resources
// ready to connect to another sender
void SpoutSDK::ReleaseReceiver()
{
	if (!m_bInitialized)
		return;

	// Restore the starting sender name if the user specified one in SetReceiverName
	if (m_SenderNameSetup[0]) {
		strcpy_s(m_SenderName, 256, m_SenderNameSetup);
	}
	else {
		m_SenderName[0] = 0;
	}

	// Wait 4 frames in case the same sender opens again
	Sleep(67);

	// Close the named access mutex and frame counting semaphore.
	frame.CloseAccessMutex();
	frame.CleanupFrameCount();

	// Zero width and height so that they are reset when a sender is found
	m_Width = 0;
	m_Height = 0;

	// Reset the received sender texture
	if (m_pSharedTexture)
		m_pSharedTexture->Release();
	m_pSharedTexture = nullptr;
	m_dxShareHandle = nullptr;
	// Reset receiver detected share mode
	m_bSenderCPUmode = false;

	// Release staging textures if they have been used
	if (m_pStaging[0]) spoutdx.ReleaseDX11Texture(spoutdx.GetDX11Device(), m_pStaging[0]);
	if (m_pStaging[1]) spoutdx.ReleaseDX11Texture(spoutdx.GetDX11Device(), m_pStaging[1]);
	m_pStaging[0] = nullptr;
	m_pStaging[1] = nullptr;
	m_Index = 0;
	m_NextIndex = 0;

	m_bConnected = false;
	m_bInitialized = false;

}



//---------------------------------------------------------
//	Receive sender shared texture
//
//  The texture can then be accessed using :
//		BindSharedTexture();
//		UnBindSharedTexture();
//		GetSharedTextureID();
//
bool SpoutSDK::ReceiveTexture()
{
	return ReceiveTexture(0, 0);
}

//---------------------------------------------------------
// Receive OpenGL texture
//
//	o Connect to a sender
//	o Set class variables for sender name, width and height
//  o If the sender has changed size, set a flag for the application to update receiving texture
//    The texture must be RGBA of dimension (width * height)
//    and can be re-allocated is IsUpdated() returns true
//  o Copy the sender shared texture to the user texture
//
bool SpoutSDK::ReceiveTexture(GLuint TextureID, GLuint TextureTarget, bool bInvert, GLuint HostFbo)
{
	// Return if flagged for update
	// The update flag is reset when the receiving application calls IsUpdated()
	if (m_bUpdated) {
		return true;
	}

	// Make sure OpenGL and DirectX are initialized
	if (!OpenSpout()) {
		return false;
	}

	// Try to receive texture details from a sender
	if (ReceiveSenderData()) {

		// Was the shared texture pointer retrieved ?
		if (!m_pSharedTexture) {
			return false;
		}

		// Found a sender
		// The sender name, width, height, format, shared texture handle
		// and shared texture pointer have been retrieved
		// Let the application know
		m_bConnected = true;

		if (m_bUpdated) {
			// If the sender is new or changed, reset shared textures and
			// return to update the receiving texture.
			// The application detects the change with IsUpdated().
			if (m_bUseGLDX) {
				// CreateInterop set "true" for receiver
				if (!CreateInterop(m_Width, m_Height, m_dwFormat, true)) {
					return false;
				}
			}
		}

		if (m_bUseGLDX) {
			// 3840x2160 60 fps - 0.45 msec/frame
			ReadGLDXtexture(TextureID, TextureTarget, m_Width, m_Height, bInvert, HostFbo);
		}
		else if (m_bAuto) {
			// 3840x2160 33 fps - 5-7 msec/frame
			ReadDX11texture(TextureID, TextureTarget, m_Width, m_Height, bInvert, HostFbo);
		}

	} // endif sender exists
	else {
		// ReceiveSenderData fails if there is no sender or the connected sender closed.
		ReleaseReceiver();
		// Let the application know.
		m_bConnected = false;
	}

	return m_bConnected;
}

//---------------------------------------------------------
bool SpoutSDK::ReceiveImage(char* Sendername, unsigned int &width, unsigned int &height,
	unsigned char* pixels, GLenum glFormat, bool bInvert, GLuint HostFBO)
{
	if (ReceiveImage(pixels, glFormat, bInvert, HostFBO)) {
		strcpy_s(Sendername, 256, m_SenderName);
		width = m_Width;
		height = m_Height;
		return true;
	}
	return false;
}

//---------------------------------------------------------
// Check for sender change
bool SpoutSDK::IsUpdated()
{
	bool bRet = m_bUpdated;
	m_bUpdated = false; // Reset the update flag
	return bRet;
}

//---------------------------------------------------------
bool SpoutSDK::IsConnected()
{
	return m_bConnected;
}

//---------------------------------------------------------
bool SpoutSDK::IsFrameNew()
{
	return m_bNewFrame;
}

//---------------------------------------------------------
DWORD SpoutSDK::GetSenderFormat()
{
	return m_dwFormat;
}

//---------------------------------------------------------
const char * SpoutSDK::GetSenderName()
{
	return m_SenderName;
}

//---------------------------------------------------------
unsigned int SpoutSDK::GetSenderWidth()
{
	return m_Width;
}

//---------------------------------------------------------
unsigned int SpoutSDK::GetSenderHeight()
{
	return m_Height;

}

//---------------------------------------------------------
double SpoutSDK::GetSenderFps()
{
	return frame.GetSenderFps();
}

//---------------------------------------------------------
long SpoutSDK::GetSenderFrame()
{
	return frame.GetSenderFrame();
}

//---------------------------------------------------------
HANDLE SpoutSDK::GetSenderHandle()
{
	return m_dxShareHandle;
}

//---------------------------------------------------------
bool SpoutSDK::GetSenderCPUmode()
{
	return m_bSenderCPUmode;
}

//---------------------------------------------------------
void SpoutSDK::SelectSender()
{
	SelectSenderPanel();
}

//
// 2.006 compatibility
//

//---------------------------------------------------------
bool SpoutSDK::CreateReceiver(char* sendername, unsigned int &width, unsigned int &height, bool bUseActive)
{
	// Make sure OpenGL and DirectX are initialized
	if (!OpenSpout())
		return false;

	// The DX11 graphics adapter could be different, so re-test compatibility for GL/DX
	CleanupInterop();
	if (!GLDXready()) {
		SpoutLogWarning("SpoutSDK::CreateReceiver - Graphics not GL/DX compatible. Switching to CPU share mode");
		m_bUseGLDX = false;
	}

	// Use the active sender if the user wants it or the sender name is not set
	if (bUseActive || sendername[0] == 0) {
		m_SenderNameSetup[0] = 0;
	}
	else {
		// Try to find the sender with the name sent
		strcpy_s(m_SenderNameSetup, 256, sendername);
	}

	if (ReceiveSenderData()) {
		// The sender name, width, height, format, shared texture handle
		// and shared texture pointer have been retrieved.
		if (m_bUpdated) {
			// If the sender is new or changed, create or re-create interop
			if (m_bUseGLDX) {
				// Flag "true" for receive
				if (!CreateInterop(m_Width, m_Height, m_dwFormat, true))
					return false;
			}
			// 2.006 receivers check for changed sender size
			m_bUpdated = false;
		}
		strcpy_s(sendername, 256, m_SenderName);
		width = m_Width;
		height = m_Height;

		return true;
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::CheckReceiver(char* name, unsigned int &width, unsigned int &height, bool &bConnected)
{
	if (ReceiveSenderData()) {
		strcpy_s(name, 256, m_SenderName);
		width = m_Width;
		height = m_Height;
		bConnected = m_bConnected;
		return true;
	}
	return false;
}

//---------------------------------------------------------
bool SpoutSDK::ReceiveTexture(char* name, unsigned int &width, unsigned int &height,
	GLuint TextureID, GLuint TextureTarget, bool bInvert, GLuint HostFBO)
{
	if (ReceiveTexture(TextureID, TextureTarget, bInvert, HostFBO)) {

		// 2.006 receivers have to restart for a new sender name
		if (m_SenderName[0] && strcmp(m_SenderName, name) != 0) {
			return false;
		}

		strcpy_s(name, 256, m_SenderName);
		width = m_Width;
		height = m_Height;
		return true;
	}

	return false;

}

//---------------------------------------------------------
bool SpoutSDK::ReceiveImage(unsigned char *pixels, GLenum glFormat, bool bInvert, GLuint HostFbo)
{
	// Return if flagged for update
	// The update flag is reset when the receiving application calls IsUpdated()
	if (m_bUpdated) {
		return true;
	}

	// Make sure OpenGL and DirectX are initialized
	if (!OpenSpout())
		return false;

	// Only RGBA, BGRA, RGB, BGR supported
	if (!(glFormat == GL_RGBA || glFormat == GL_BGRA_EXT || glFormat == GL_RGB || glFormat == GL_BGR_EXT))
		return false;

	// Check for BGRA support
	GLenum glformat = glFormat;
	if (!m_bBGRAavailable) {
		// If the bgra extensions are not available and the user
		// provided GL_BGR_EXT or GL_BGRA_EXT do not use them
		if (glFormat == GL_BGR_EXT) glformat = GL_RGB; // GL_BGR_EXT
		if (glFormat == GL_BGRA_EXT) glformat = GL_RGBA; // GL_BGRA_EXT
	}

	// Try to receive texture details from a sender
	if (ReceiveSenderData()) {

		// The sender name, width, height, format, shared texture handle and pointer have been retrieved.
		if (m_bUpdated) {

			// Was the shared texture pointer retrieved ?
			if (!m_pSharedTexture)
				return false;

			// If the sender is new or changed, return to update the receiving texture.
			// The application detects the change with IsUpdated().
			if (m_bUseGLDX) {
				// Flag "true" for receive
				if (!CreateInterop(m_Width, m_Height, m_dwFormat, true))
					return false;
			}

			return true;
		}

		// The receiving pixel buffer is created after the first update
		// So check here instead of at the beginning
		if (!pixels)
			return false;

		//
		// Found a sender
		//
		// Read the shared texture into the pixel buffer
		// Copy functions handle the formats supported
		if (m_bUseGLDX && m_bPBOavailable) {
			// Read pixels using OpenGL via PBO
			// PBO (UnloadTexturePixels)
			// 1920x1080 RGB 1.4 msec/frame RGBA 1.6 msec/frame
			// 3840x2160 RGB 5 msec/frame RGBA 6 msec/frame
			// FBO (ReadTextureData) - slower than DirectX method
			// 3840x2160 RGB 30-60 msec/frame RGBA 30-60 msec/frame
			ReadGLDXpixels(pixels, m_Width, m_Height, glformat, bInvert, HostFbo);
		}
		else if (m_bAuto) {
			// Read pixels via DX11 staging textures to an rgba or rgb buffer
			// 1920x1080 RGB 7 msec/frame RGBA 2 msec/frame
			// 3840x2160 RGB 30 msec/frame RGBA 9 msec/frame
			ReadDX11pixels(pixels, m_Width, m_Height, glformat, bInvert);
		}
		m_bConnected = true;
	} // sender exists
	else {
		// There is no sender or the connected sender closed.
		ReleaseReceiver();
		// Let the application know.
		m_bConnected = false;
	}

	// ReceiveImage fails if there is no sender or the connected sender closed.
	return m_bConnected;

} // end ReceiveImage

//---------------------------------------------------------
bool SpoutSDK::SelectSenderPanel(const char *message)
{
	HANDLE hMutex1 = NULL;
	HMODULE module = NULL;
	char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH];
	char UserMessage[512];

	if (message != NULL && message[0] != 0)
		strcpy_s(UserMessage, 512, message); // could be an arg or a user message
	else
		UserMessage[0] = 0; // make sure SpoutPanel does not see an un-initialized string

	// The selected sender is then the "Active" sender and this receiver switches to it.
	// If Spout is not installed, SpoutPanel.exe has to be in the same folder
	// as this executable. This rather complicated process avoids having to use a dialog
	// which causes problems with host GUI messaging.

	// First find if there has been a Spout installation >= 2.002 with an install path for SpoutPanel.exe
	if (!ReadPathFromRegistry(HKEY_CURRENT_USER, "Software\\Leading Edge\\SpoutPanel", "InstallPath", path)) {
		// Path not registered so find the path of the host program
		// where SpoutPanel should have been copied
		module = GetModuleHandle(NULL);
		GetModuleFileNameA(module, path, MAX_PATH);
		_splitpath_s(path, drive, MAX_PATH, dir, MAX_PATH, fname, MAX_PATH, NULL, 0);
		_makepath_s(path, MAX_PATH, drive, dir, "SpoutPanel", ".exe");
		// Does SpoutPanel.exe exist in this path ?
		if (!PathFileExistsA(path)) {
			// Try the current working directory
			if (_getcwd(path, MAX_PATH)) {
				strcat_s(path, MAX_PATH, "\\SpoutPanel.exe");
				// Does SpoutPanel exist here?
				if (!PathFileExistsA(path)) {
					SpoutLogWarning("spoutDX::SelectSender - SpoutPanel path not found");
					return false;
				}
			}
		}
	}

	// Check whether the panel is already running
	// Try to open the application mutex.
	hMutex1 = OpenMutexA(MUTEX_ALL_ACCESS, 0, "SpoutPanel");
	if (!hMutex1) {
		// No mutex, so not running, so can open it
		// Use ShellExecuteEx so we can test its return value later
		ZeroMemory(&m_ShExecInfo, sizeof(m_ShExecInfo));
		m_ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		m_ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		m_ShExecInfo.hwnd = NULL;
		m_ShExecInfo.lpVerb = NULL;
		m_ShExecInfo.lpFile = (LPCSTR)path;
		m_ShExecInfo.lpParameters = UserMessage;
		m_ShExecInfo.lpDirectory = NULL;
		m_ShExecInfo.nShow = SW_SHOW;
		m_ShExecInfo.hInstApp = NULL;
		ShellExecuteExA(&m_ShExecInfo);

		//
		// The flag "m_bSpoutPanelOpened" is set here to indicate that the user
		// has opened the panel to select a sender. This flag is local to 
		// this process so will not affect any other receiver instance
		// Then when the selection panel closes, sender name is tested
		//
		m_bSpoutPanelOpened = true;
	}
	else {
		// The mutex exists, so another instance is already running.
		// Find the SpoutPanel window and bring it to the top.
		// SpoutPanel is opened as topmost anyway but pop it to
		// the front in case anything else has stolen topmost.
		HWND hWnd = FindWindowA(NULL, (LPCSTR)"SpoutPanel");
		if (hWnd && IsWindow(hWnd)) {
			SetForegroundWindow(hWnd);
			// prevent other windows from hiding the dialog
			// and open the window wherever the user clicked
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
		}
		else if (path[0]) {
			// If the window was not found but the mutex exists
			// and SpoutPanel is installed, it has crashed.
			// Terminate the process and the mutex or the mutex will remain
			// and SpoutPanel will not be started again.
			PROCESSENTRY32 pEntry;
			pEntry.dwSize = sizeof(pEntry);
			bool done = false;
			// Take a snapshot of all processes and threads in the system
			HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
			if (hProcessSnap == INVALID_HANDLE_VALUE) {
				SpoutLogError("spoutDX::OpenSpoutPanel - CreateToolhelp32Snapshot error");
			}
			else {
				// Retrieve information about the first process
				BOOL hRes = Process32First(hProcessSnap, &pEntry);
				if (!hRes) {
					SpoutLogError("spoutDX::OpenSpoutPanel - Process32First error");
					CloseHandle(hProcessSnap);
				}
				else {
					// Look through all processes
					while (hRes && !done) {
						int value = _tcsicmp(pEntry.szExeFile, _T("SpoutPanel.exe"));
						if (value == 0) {
							HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
							if (hProcess != NULL) {
								// Terminate SpoutPanel and it's mutex
								TerminateProcess(hProcess, 9);
								CloseHandle(hProcess);
								done = true;
							}
						}
						if (!done)
							hRes = Process32Next(hProcessSnap, &pEntry); // Get the next process
						else
							hRes = NULL; // found SpoutPanel
					}
					CloseHandle(hProcessSnap);
				}
			}
			// Now SpoutPanel will start the next time the user activates it
		} // endif SpoutPanel crashed
	} // endif SpoutPanel already open

	// If we opened the mutex, close it now or it is never released
	if (hMutex1) CloseHandle(hMutex1);

	return true;

} // end SelectSenderPanel


// Legacy OpenGL Draw function
#ifdef legacyOpenGL

//---------------------------------------------------------
bool SpoutSDK::DrawSharedTexture(float max_x, float max_y, float aspect, bool bInvert, GLuint HostFBO)
{
	if (!m_hInteropDevice || !m_hInteropObject)
		return false;

	bool bRet = false;

	// Wait for access to the shared texture
	if (frame.CheckTextureAccess(m_pSharedTexture)) {
		// go ahead and access the shared texture to draw it
		if (LockInteropObject(m_hInteropDevice, &m_hInteropObject) == S_OK) {
			SaveOpenGLstate(m_Width, m_Height);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_glTexture); // bind shared texture
			glColor4f(1.f, 1.f, 1.f, 1.f);
			// Tried to convert to vertex array, but Processing crash
			glBegin(GL_QUADS);
			if (bInvert) {
				glTexCoord2f(0.0, max_y);	glVertex2f(-aspect, -1.0); // lower left
				glTexCoord2f(0.0, 0.0);	glVertex2f(-aspect, 1.0); // upper left
				glTexCoord2f(max_x, 0.0);	glVertex2f(aspect, 1.0); // upper right
				glTexCoord2f(max_x, max_y);	glVertex2f(aspect, -1.0); // lower right
			}
			else {
				glTexCoord2f(0.0, 0.0);	glVertex2f(-aspect, -1.0); // lower left
				glTexCoord2f(0.0, max_y);	glVertex2f(-aspect, 1.0); // upper left
				glTexCoord2f(max_x, max_y);	glVertex2f(aspect, 1.0); // upper right
				glTexCoord2f(max_x, 0.0);	glVertex2f(aspect, -1.0); // lower right
			}
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			RestoreOpenGLstate();
			UnlockInteropObject(m_hInteropDevice, &m_hInteropObject); // unlock dx object
			bRet = true;
		} // lock failed
		// Release mutex and allow access to the texture
		frame.AllowTextureAccess(m_pSharedTexture);
	} // mutex lock failed

	return bRet;

} // end DrawSharedTexture
#endif


//
// Protected functions
//

//---------------------------------------------------------
void SpoutSDK::InitReceiver(const char * SenderName, unsigned int width, unsigned int height, DWORD dwFormat)
{
	SpoutLogNotice("SpoutSDK::InitReceiver(%s, %d x %d)", SenderName, width, height);

	// Create a named sender mutex for access to the sender's shared texture
	frame.CreateAccessMutex(SenderName);

	// Enable frame counting to get the sender frame number and fps
	frame.EnableFrameCount(SenderName);

	// Set class globals
	strcpy_s(m_SenderName, 256, SenderName);
	m_Width = width;
	m_Height = height;
	m_dwFormat = dwFormat;

	// The graphics adapter could be different, so re-test compatibility for GL/DX
	CleanupInterop();
	if (!GLDXready()) {
		SpoutLogWarning("SpoutSDK::InitReceiver - Graphics not GL/DX compatible.");
		// GLDXready has set m_bUseGLDX false;
	}

	// Get graphics adapter number, index and name
	m_AdapterNumber = GetNumAdapters();
	m_AdapterIndex = GetAdapter();
	GetAdapterName(m_AdapterIndex, m_AdapterName, 256);

	// printf("SpoutSDK::CreateReceiver(%s, %d x %d) - format = %d\n", SenderName, width, height, m_dwFormat);

	m_bInitialized = true;

}

//---------------------------------------------------------
//	o Connect to a sender and inform the application to update texture dimensions
//	o Check for user sender selection
//  o Receive texture details from the sender for write to the user texture
//  o Retrieve width, height, format, share handle and texture pointer
bool SpoutSDK::ReceiveSenderData()
{
	m_bUpdated = false;

	// Initialization is recorded in this class for sender or receiver
	// m_Width or m_Height are established when the receiver connects to a sender
	char sendername[256];
	strcpy_s(sendername, 256, m_SenderName);

	// Check the entered Sender name to see if it exists
	if (sendername[0] == 0) {
		// Passed name was null, so find the active sender
		if (!GetActiveSender(sendername))
			return false; // No sender
	}

	// If SpoutPanel has been opened, the active sender name could be different
	if (CheckSpoutPanel(sendername, 256)) {
		// Disable the setup name
		m_SenderNameSetup[0] = 0;
	}

	// Now we have either an existing sender name or the active sender name

	// Save current sender name and dimensions to test for change
	unsigned int width = m_Width;
	unsigned int height = m_Height;
	DWORD dwFormat = m_dwFormat;
	HANDLE dxShareHandle = m_dxShareHandle;

	// Try to get the sender information
	// Retreive width, height, sharehandle and format.
	SharedTextureInfo info;
	if (sendernames.getSharedInfo(sendername, &info)) {

		// Memoryshare not supported (no texture share handle)
		if (info.shareHandle == 0) {
			ReleaseReceiver();
			return false;
		}

		// Received sender share mode
		if (info.partnerId & 0x80000000) // CPU mode - top bit of 32 bit partner ID field
			m_bSenderCPUmode = true;
		else
			m_bSenderCPUmode = false; // Texture share default

		width = info.width;
		height = info.height;
		dxShareHandle = (HANDLE)(LongToHandle((long)info.shareHandle));
		dwFormat = info.format;

		// printf("Found sender [%s] (%dx%d) format = %d info.partnerId = 0x%.7X\n", sendername, width, height, dwFormat, info.partnerId);

		// Compatible DX9 formats
		// 21 =	D3DFMT_A8R8G8B8
		// 22 = D3DFMT_X8R8G8B8
		if (dwFormat == 21 || dwFormat == 21) {
			// Create a DX11 receiving texture with compatible format
			dwFormat = (DWORD)DXGI_FORMAT_B8G8R8A8_UNORM;
		}

		// The shared texture handle will be different
		//   o for a new sender
		//   o for texture size or format change
		if (dxShareHandle != m_dxShareHandle) {

			// printf("Found new sender (%dx%d) format = %d\n", width, height, dwFormat);

			// Release everything and start again
			ReleaseReceiver();

			// Update the sender share handle
			m_dxShareHandle = dxShareHandle;

			// Get a new shared texture pointer (m_pSharedTexture)
			if (!spoutdx.OpenDX11shareHandle(spoutdx.GetDX11Device(), &m_pSharedTexture, dxShareHandle)) {

				// If this fails something is wrong
				// the sender graphics adapter might be different or some other reason
				SpoutLogWarning("SpoutSDK::ReceiveSenderData - could not retrieve sender texture from share handle");

				// Retain the share handle so we don't query it again.
				// m_pSharedTexture is null but will not be used.
				// Return true and wait until another sender is selected.
				return true;

			}

			// Initialize again with the newly connected sender values
			InitReceiver(sendername, width, height, dwFormat);

			m_bUpdated = true; // Return to update the receiving texture or image

		}

		// Connected and intialized
		// Sender name, width, height, format, texture pointer and share handle have been retrieved

		// The application can now access and copy the sender texture
		return true;

	} // end find sender

	// There is no sender or the connected sender closed
	return false;

}



//---------------------------------------------------------
// Check whether SpoutPanel opened and return the new sender name
bool SpoutSDK::CheckSpoutPanel(char *sendername, int maxchars)
{
	// If SpoutPanel has been activated, test if the user has clicked OK
	if (m_bSpoutPanelOpened) { // User has activated spout panel

		SharedTextureInfo TextureInfo;
		HANDLE hMutex = NULL;
		DWORD dwExitCode;
		char newname[256];
		bool bRet = false;

		// Must find the mutex to signify that SpoutPanel has opened
		// and then wait for the mutex to close
		hMutex = OpenMutexA(MUTEX_ALL_ACCESS, 0, "SpoutPanel");

		// Has it been activated 
		if (!m_bSpoutPanelActive) {
			// If the mutex has been found, set the active flag true and quit
			// otherwise on the next round it will test for the mutex closed
			if (hMutex) m_bSpoutPanelActive = true;
		}
		else if (!hMutex) { // It has now closed
			m_bSpoutPanelOpened = false; // Don't do this part again
			m_bSpoutPanelActive = false;
			// call GetExitCodeProcess() with the hProcess member of
			// global SHELLEXECUTEINFO to get the exit code from SpoutPanel
			if (m_ShExecInfo.hProcess) {
				GetExitCodeProcess(m_ShExecInfo.hProcess, &dwExitCode);
				// Only act if exit code = 0 (OK)
				if (dwExitCode == 0) {
					// SpoutPanel has been activated and OK clicked
					// Test the active sender which should have been set by SpoutPanel
					newname[0] = 0;
					if (!sendernames.GetActiveSender(newname)) {
						// Otherwise the sender might not be registered.
						// SpoutPanel always writes the selected sender name to the registry.
						if (ReadPathFromRegistry(HKEY_CURRENT_USER, "Software\\Leading Edge\\SpoutPanel", "Sendername", newname)) {
							// Register the sender if it exists
							if (newname[0] != 0) {
								if (sendernames.getSharedInfo(newname, &TextureInfo)) {
									// Register in the list of senders and make it the active sender
									sendernames.RegisterSenderName(newname);
									sendernames.SetActiveSender(newname);
								}
							}
						}
					}
					// Now do we have a valid sender name ?
					if (newname[0] != 0) {
						// Pass back the new name
						strcpy_s(sendername, maxchars, newname);
						bRet = true;
					} // endif valid sender name
				} // endif SpoutPanel OK
			} // got the exit code
		} // endif no mutex so SpoutPanel has closed
		// If we opened the mutex, close it now or it is never released
		if (hMutex) CloseHandle(hMutex);
		return bRet;
	} // SpoutPanel has not been opened

	return false;

}
