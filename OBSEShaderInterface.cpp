/* Version: MPL 1.1/LGPL 3.0
 *
 * "The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Code is the Oblivion Graphics Extender, short OBGE.
 *
 * The Initial Developer of the Original Code is
 * Ethatron <niels@paradice-insight.us>. Portions created by The Initial
 * Developer are Copyright (C) 2011 The Initial Developer.
 * All Rights Reserved.
 *
 * Contributor(s):
 *  Timeslip (Version 1)
 *  scanti (Version 2)
 *  IlmrynAkios (Version 3)
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU Library General Public License Version 3 license (the
 * "LGPL License"), in which case the provisions of LGPL License are
 * applicable instead of those above. If you wish to allow use of your
 * version of this file only under the terms of the LGPL License and not
 * to allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL License.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under either the MPL or the LGPL License."
 */

#include "OBSEShaderInterface.h"
#include "nodes/NiDX9Renderer.h"
#include "nodes/NiCamera.h"
#include "Rendering.h"
#include "DepthBufferHook.h"
#include "OBGE fork/Sky.h"
#include "nodes/NiBillboardNode.h"
#include "nodes/NiVector4.h"
#include "ScreenElements.h"
#include "GlobalSettings.h"

#include "ShaderManager.h"
#include "EffectManager.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "LODManager.h"

#include "D3D9.hpp"
#include "D3D9Device.hpp"

#pragma warning(disable : 4996)

static global<bool> UseSave(true, NULL, "Serialization", "bSaveData");
static global<bool> UseLoad(true, NULL, "Serialization", "bLoadData");
static global<bool> EnableInterOp(false, NULL, "PluginInterOp", "bEnableInterOp");
static global<bool> SaveFix(false, NULL, "Effects", "bNoShadersInMenus");
static global<bool> TailEffects(false, NULL, "Effects", "bTailEffects");
static global<bool> Enabled(true, NULL, "General", "bEnabled");
static global<bool> ExtHDR(false, "Oblivion.ini", "BlurShaderHDR", "bDoHighDynamicRange");
static global<bool> IntHDR(false, "Oblivion.ini", "BlurShaderHDRInterior", "bDoHighDynamicRange");
static global<bool> Bloom(false, "Oblivion.ini", "BlurShader", "bUseBlurShader");

// Uses code from OBGE by Timeslip.

OBSEShaderInterface *OBSEShaderInterface::Singleton = NULL;

// TO DO : Needs tidying.

bool LostDevice(bool stage, void *parameters) {
  NiTListBase<SpoofShader>::Node *CurrentNode;

  _MESSAGE("Lost device handler:");

  if (OBSEShaderInterface::Singleton) {
    CurrentNode = obImageSpaceShaderList->p->EffectList.start;

    while (CurrentNode) {
      if (CurrentNode->data->IsSpoofShader()) {
        if (stage)
          CurrentNode->data->DeviceLost();
        else
          CurrentNode->data->DeviceReset();
      }

      CurrentNode = CurrentNode->next;
    }
  }

  return true;
}

OBSEShaderInterface *OBSEShaderInterface::GetSingleton() {
  if (!Singleton) {
    Singleton = new OBSEShaderInterface();

    v1_2_416::GetRenderer()->RegisterLostDeviceCallback(LostDevice, NULL);

    // Need to increase the ref count of the spoof shader otherwise the game engine will try to delete it. Of course
    // as I haven't written a destructor it will fail.
    Singleton->RefCount++;

    if (TailEffects.Get())
      obImageSpaceShaderList->p->AddTail(Singleton);	// put after alllll other effects
    else
      obImageSpaceShaderList->p->AddHead(Singleton);	// put before allll other effects

    Singleton->InitializeEffects();
    Singleton->ActivateShader = true;

    _MESSAGE("Added to list OK.");
  }

  return Singleton;
}

void OBSEShaderInterface::ShaderCode(IDirect3DDevice9 *D3DDevice, IDirect3DSurface9 *RenderTo, IDirect3DSurface9 *RenderFrom, DeviceInfo *Info) {
//if (Info->AltRenderTarget)
//  _MESSAGE("Alt Render target - width = %i, height = %i", Info->Width, Info->Height);

  if (Info->AltRenderTarget && (SaveFix.data || ((Info->Height) == 256 && (Info->Width == 256)))) {
    D3DDevice->StretchRect(RenderFrom, NULL, RenderTo, NULL, D3DTEXF_NONE);
    return;
  }

#ifndef	OBGE_NOSHADER
  enum OBGEPass previousPass = currentPass;
  currentPass = OBGEPASS_CUSTOM;
#endif

  LODManager::GetSingleton()->Render(D3DDevice, RenderTo, RenderFrom);

#ifndef	OBGE_NOSHADER
  currentPass = OBGEPASS_EFFECTS;
#endif

  EffectManager::GetSingleton()->Render(D3DDevice, RenderTo, RenderFrom);
  HUDManager::GetSingleton()->Render(D3DDevice);

#ifndef	OBGE_NOSHADER
  currentPass = previousPass;
#endif

  /*
  	if(EnableInterOp.data)
  	{
  		struct INTEROP {
  			UInt32		version;
  			IDirect3DDevice9 *D3DDevice;
  			IDirect3DSurface9 *RenderTo;
  			UInt32		width;
  			UInt32		height;
  			D3DCAPS9	*DeviceCaps;
  		} InterOp;

  		InterOp.version=1;
  		InterOp.D3DDevice=D3DDevice;
  		InterOp.RenderTo=RenderTo;
  		InterOp.width=Info->Width;
  		InterOp.height=Info->Height;
  		InterOp.DeviceCaps=Info->Caps;

  		GetMessaging()->Dispatch(GetHandle(),'REND',(void *)&InterOp,sizeof(InterOp),NULL);
  	}
  */

  // I'll keep the font stuff in as I might need it later for debugging purposes.

  /*
  	if (!pFont)
  	{
  		hr=D3DXCreateFontIndirectA(D3DDevice,&FontDescription,&pFont);
  		if (hr!=S_OK || !pFont)
  		{
  			_MESSAGE("ERROR CREATING FONT");
  			return;
  		}
  	}

  	if (!pFont2)
  	{
  		hr=D3DXCreateFontIndirectA(D3DDevice,&FontDescription2,&pFont2);
  		if (hr!=S_OK || !pFont2)
  		{
  			_MESSAGE("ERROR CREATING FONT");
  			return;
  		}
  	}
  */
}

void OBSEShaderInterface::DeviceLost() {
  _MESSAGE("Calling Lost Device");

  if (pFont ) pFont ->OnLostDevice();
  if (pFont2) pFont2->OnLostDevice();

     LODManager::GetSingleton()->OnLostDevice();
  ShaderManager::GetSingleton()->OnLostDevice();
  EffectManager::GetSingleton()->OnLostDevice();
}

void OBSEShaderInterface::DeviceReset() {
  _MESSAGE("Calling Reset Device");

  if (pFont ) pFont ->OnResetDevice();
  if (pFont2) pFont2->OnResetDevice();

     LODManager::GetSingleton()->OnResetDevice();
  ShaderManager::GetSingleton()->OnResetDevice();
  EffectManager::GetSingleton()->OnResetDevice();
}

void OBSEShaderInterface::DeviceRelease() {
  _MESSAGE("Calling Release Device");

  if (pFont ) { while (pFont ->Release()) {} pFont  = NULL; }
  if (pFont2) { while (pFont2->Release()) {} pFont2 = NULL; }

     LODManager::GetSingleton()->OnReleaseDevice();
  ShaderManager::GetSingleton()->OnReleaseDevice();
  EffectManager::GetSingleton()->OnReleaseDevice();
//LostDepthBuffer(true,NULL);

  delete     LODManager::GetSingleton();
//delete  ShaderManager::GetSingleton();	// unsafe!
  delete  EffectManager::GetSingleton();
  delete    MeshManager::GetSingleton();
  delete TextureManager::GetSingleton();
//delete MemoryDumpString;
}

void OBSEShaderInterface::InitializeEffects(void) {
  DebugOn = false;

  pFont = NULL;
  pFont2 = NULL;

  FontRect.top = 10;
  FontRect.bottom = 490;
  FontRect.left = 10;
  FontRect.right = 790;

  FontDescription.Height = -16;
  FontDescription.Width = 0;
  FontDescription.Weight = FW_NORMAL;
  FontDescription.MipLevels = 1;
  FontDescription.Italic = false;
  FontDescription.CharSet = ANSI_CHARSET;
  FontDescription.OutputPrecision = OUT_DEFAULT_PRECIS;
  FontDescription.Quality = ANTIALIASED_QUALITY;
  FontDescription.PitchAndFamily = FF_DONTCARE | FIXED_PITCH;
  strcpy(FontDescription.FaceName, "Courier New");

  FontDescription2.Height = -16;
  FontDescription2.Width = 0;
  FontDescription2.Weight = FW_HEAVY;
  FontDescription2.MipLevels = 1;
  FontDescription2.Italic = false;
  FontDescription2.CharSet = ANSI_CHARSET;
  FontDescription2.OutputPrecision = OUT_DEFAULT_PRECIS;
  FontDescription2.Quality = ANTIALIASED_QUALITY;
  FontDescription2.PitchAndFamily = FF_DONTCARE | FIXED_PITCH;
  strcpy(FontDescription2.FaceName, "Courier New");

  FontColor = D3DCOLOR_RGBA(255, 255, 255, 255);
  FontColor2 = D3DCOLOR_RGBA(0, 0, 0, 255);

  EffectManager *EffectMan = EffectManager::GetSingleton();

  EffectMan->InitializeBuffers();
  EffectMan->InitializeFrameTextures();
  EffectMan->LoadEffectList();

  LODManager *LODMan = LODManager::GetSingleton();

  LODMan->InitializeBuffers();

//MemoryDumpString=new TextBuffer(10000);

  return;
}

void OBSEShaderInterface::NewGame() {
  TextureManager *TexMan    = TextureManager::GetSingleton();
  MeshManager    *MeshMan   =    MeshManager::GetSingleton();
  EffectManager  *EffectMan =  EffectManager::GetSingleton();

  TexMan->NewGame();
  MeshMan->NewGame();
  EffectMan->NewGame();
  EffectMan->LoadEffectList();
}

void OBSEShaderInterface::LoadGame(OBSESerializationInterface *Interface) {
  if (IsEnabled()) {
    NewGame();

    if (UseLoad.data) {
      TextureManager::GetSingleton()->LoadGame(Interface);
         MeshManager::GetSingleton()->LoadGame(Interface);
       EffectManager::GetSingleton()->LoadGame(Interface);
    }
    else {
      _MESSAGE("Loading disabled in INI file.");
    }
  }
}

void OBSEShaderInterface::SaveGame(OBSESerializationInterface *Interface) {
  if (UseSave.data) {
    TextureManager::GetSingleton()->SaveGame(Interface);
       MeshManager::GetSingleton()->SaveGame(Interface);
     EffectManager::GetSingleton()->SaveGame(Interface);
  }
  else {
    _MESSAGE("Saving disabled in INI file.");
  }
}

void SetMessaging(OBSEMessagingInterface *Interface, PluginHandle Handle) {
  messanger = Interface;
  handle = Handle;
  return;
}

OBSEMessagingInterface *GetMessaging(void) {
  return messanger;
}

PluginHandle GetHandle(void) {
  return handle;
}

bool IsEnabled() {
  return Enabled.data;
}

bool IsHDR() {
  return !Bloom.Get() && (ExtHDR.Get() || IntHDR.Get());
}

bool IsBloom() {
  return Bloom.Get();
}

bool IsPlain() {
  return !(IsHDR() || IsBloom());
}
