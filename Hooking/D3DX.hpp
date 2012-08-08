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

#ifndef	HOOKING_D3DX_HPP
#define	HOOKING_D3DX_HPP

#include <d3dx9.h>

/* ----------------------------------------------------------------------------- */
// Tracker
extern IDirect3DBaseTexture9 *lastOBGEDirect3DBaseTexture9;

// Function pointer types.
typedef HRESULT (FAR WINAPI *D3DXSaveTextureToFileA_t)(LPCTSTR pDestFile,
							D3DXIMAGE_FILEFORMAT DestFormat,
							LPDIRECT3DBASETEXTURE9 pSrcTexture,
							const PALETTEENTRY *pSrcPalette);

// Function prototypes.
HRESULT FAR WINAPI OBGESaveTextureToFileA(LPCTSTR pDestFile,
					  D3DXIMAGE_FILEFORMAT DestFormat,
					  LPDIRECT3DBASETEXTURE9 pSrcTexture,
					  const PALETTEENTRY *pSrcPalette);

/* ----------------------------------------------------------------------------- */

// Function pointer types.
typedef HRESULT (FAR WINAPI *D3DXCreateTextureFromFileA_t)(LPDIRECT3DDEVICE9 pDevice,
							   LPCTSTR pSrcFile,
							   LPDIRECT3DTEXTURE9 *ppTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateTextureFromFileExA_t)(LPDIRECT3DDEVICE9 pDevice,
							     LPCTSTR pSrcFile,
							     UINT Width,
							     UINT Height,
							     UINT MipLevels,
							     DWORD Usage,
							     D3DFORMAT Format,
							     D3DPOOL Pool,
							     DWORD Filter,
							     DWORD MipFilter,
							     D3DCOLOR ColorKey,
							     D3DXIMAGE_INFO *pSrcInfo,
							     PALETTEENTRY *pPalette,
							     LPDIRECT3DTEXTURE9 *ppTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateTextureFromFileInMemory_t)(LPDIRECT3DDEVICE9 pDevice,
								   LPCVOID pSrcData,
								   UINT SrcDataSize,
								   LPDIRECT3DTEXTURE9 *ppTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateTextureFromFileInMemoryEx_t)(LPDIRECT3DDEVICE9 pDevice,
								     LPCVOID pSrcData,
								     UINT SrcDataSize,
								     UINT Width,
								     UINT Height,
								     UINT MipLevels,
								     DWORD Usage,
								     D3DFORMAT Format,
								     D3DPOOL Pool,
								     DWORD Filter,
								     DWORD MipFilter,
								     D3DCOLOR ColorKey,
								     D3DXIMAGE_INFO *pSrcInfo,
								     PALETTEENTRY *pPalette,
								     LPDIRECT3DTEXTURE9 *ppTexture);

// Function prototypes.
HRESULT FAR WINAPI OBGECreateTextureFromFileA(LPDIRECT3DDEVICE9 pDevice,
					      LPCTSTR pSrcFile,
					      LPDIRECT3DTEXTURE9 *ppTexture);
HRESULT FAR WINAPI OBGECreateTextureFromFileExA(LPDIRECT3DDEVICE9 pDevice,
						LPCTSTR pSrcFile,
						UINT Width,
						UINT Height,
						UINT MipLevels,
						DWORD Usage,
						D3DFORMAT Format,
						D3DPOOL Pool,
						DWORD Filter,
						DWORD MipFilter,
						D3DCOLOR ColorKey,
						D3DXIMAGE_INFO *pSrcInfo,
						PALETTEENTRY *pPalette,
						LPDIRECT3DTEXTURE9 *ppTexture);
HRESULT FAR WINAPI OBGECreateTextureFromFileInMemory(LPDIRECT3DDEVICE9 pDevice,
						      LPCVOID pSrcData,
						      UINT SrcDataSize,
						      LPDIRECT3DTEXTURE9 *ppTexture);
HRESULT FAR WINAPI OBGECreateTextureFromFileInMemoryEx(LPDIRECT3DDEVICE9 pDevice,
							LPCVOID pSrcData,
							UINT SrcDataSize,
							UINT Width,
							UINT Height,
							UINT MipLevels,
							DWORD Usage,
							D3DFORMAT Format,
							D3DPOOL Pool,
							DWORD Filter,
							DWORD MipFilter,
							D3DCOLOR ColorKey,
							D3DXIMAGE_INFO *pSrcInfo,
							PALETTEENTRY *pPalette,
							LPDIRECT3DTEXTURE9 *ppTexture);

/* ----------------------------------------------------------------------------- */

// Function pointer types.
typedef HRESULT (FAR WINAPI *D3DXCreateCubeTextureFromFileA_t)(LPDIRECT3DDEVICE9 pDevice,
							   LPCTSTR pSrcFile,
							   LPDIRECT3DTEXTURE9 *ppCubeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateCubeTextureFromFileExA_t)(LPDIRECT3DDEVICE9 pDevice,
								 LPCTSTR pSrcFile,
								 UINT Size,
							     UINT MipLevels,
							     DWORD Usage,
							     D3DFORMAT Format,
							     D3DPOOL Pool,
							     DWORD Filter,
							     DWORD MipFilter,
							     D3DCOLOR ColorKey,
							     D3DXIMAGE_INFO *pSrcInfo,
							     PALETTEENTRY *pPalette,
							     LPDIRECT3DTEXTURE9 *ppCubeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateCubeTextureFromFileInMemory_t)(LPDIRECT3DDEVICE9 pDevice,
								   LPCVOID pSrcData,
								   UINT SrcDataSize,
								   LPDIRECT3DTEXTURE9 *ppCubeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateCubeTextureFromFileInMemoryEx_t)(LPDIRECT3DDEVICE9 pDevice,
								     LPCVOID pSrcData,
								     UINT SrcDataSize,
								     UINT Size,
								     UINT MipLevels,
								     DWORD Usage,
								     D3DFORMAT Format,
								     D3DPOOL Pool,
								     DWORD Filter,
								     DWORD MipFilter,
								     D3DCOLOR ColorKey,
								     D3DXIMAGE_INFO *pSrcInfo,
								     PALETTEENTRY *pPalette,
								     LPDIRECT3DTEXTURE9 *ppCubeTexture);

// Function prototypes.
HRESULT FAR WINAPI OBGECreateCubeTextureFromFileA(LPDIRECT3DDEVICE9 pDevice,
					      LPCTSTR pSrcFile,
					      LPDIRECT3DTEXTURE9 *ppCubeTexture);
HRESULT FAR WINAPI OBGECreateCubeTextureFromFileExA(LPDIRECT3DDEVICE9 pDevice,
						LPCTSTR pSrcFile,
						UINT Size,
						UINT MipLevels,
						DWORD Usage,
						D3DFORMAT Format,
						D3DPOOL Pool,
						DWORD Filter,
						DWORD MipFilter,
						D3DCOLOR ColorKey,
						D3DXIMAGE_INFO *pSrcInfo,
						PALETTEENTRY *pPalette,
						LPDIRECT3DTEXTURE9 *ppCubeTexture);
HRESULT FAR WINAPI OBGECreateCubeTextureFromFileInMemory(LPDIRECT3DDEVICE9 pDevice,
						      LPCVOID pSrcData,
						      UINT SrcDataSize,
						      LPDIRECT3DTEXTURE9 *ppCubeTexture);
HRESULT FAR WINAPI OBGECreateCubeTextureFromFileInMemoryEx(LPDIRECT3DDEVICE9 pDevice,
							LPCVOID pSrcData,
							UINT SrcDataSize,
							UINT Size,
							UINT MipLevels,
							DWORD Usage,
							D3DFORMAT Format,
							D3DPOOL Pool,
							DWORD Filter,
							DWORD MipFilter,
							D3DCOLOR ColorKey,
							D3DXIMAGE_INFO *pSrcInfo,
							PALETTEENTRY *pPalette,
							LPDIRECT3DTEXTURE9 *ppCubeTexture);

/* ----------------------------------------------------------------------------- */

// Function pointer types.
typedef HRESULT (FAR WINAPI *D3DXCreateVolumeTextureFromFileA_t)(LPDIRECT3DDEVICE9 pDevice,
							   LPCTSTR pSrcFile,
							   LPDIRECT3DTEXTURE9 *ppVolumeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateVolumeTextureFromFileExA_t)(LPDIRECT3DDEVICE9 pDevice,
							     LPCTSTR pSrcFile,
							     UINT Width,
							     UINT Height,
							     UINT Depth,
							     UINT MipLevels,
							     DWORD Usage,
							     D3DFORMAT Format,
							     D3DPOOL Pool,
							     DWORD Filter,
							     DWORD MipFilter,
							     D3DCOLOR ColorKey,
							     D3DXIMAGE_INFO *pSrcInfo,
							     PALETTEENTRY *pPalette,
							     LPDIRECT3DTEXTURE9 *ppVolumeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateVolumeTextureFromFileInMemory_t)(LPDIRECT3DDEVICE9 pDevice,
								   LPCVOID pSrcData,
								   UINT SrcDataSize,
								   LPDIRECT3DTEXTURE9 *ppVolumeTexture);
typedef HRESULT (FAR WINAPI *D3DXCreateVolumeTextureFromFileInMemoryEx_t)(LPDIRECT3DDEVICE9 pDevice,
								     LPCVOID pSrcData,
								     UINT SrcDataSize,
								     UINT Width,
								     UINT Height,
								     UINT Depth,
								     UINT MipLevels,
								     DWORD Usage,
								     D3DFORMAT Format,
								     D3DPOOL Pool,
								     DWORD Filter,
								     DWORD MipFilter,
								     D3DCOLOR ColorKey,
								     D3DXIMAGE_INFO *pSrcInfo,
								     PALETTEENTRY *pPalette,
								     LPDIRECT3DTEXTURE9 *ppVolumeTexture);

// Function prototypes.
HRESULT FAR WINAPI OBGECreateVolumeTextureFromFileA(LPDIRECT3DDEVICE9 pDevice,
					      LPCTSTR pSrcFile,
					      LPDIRECT3DTEXTURE9 *ppVolumeTexture);
HRESULT FAR WINAPI OBGECreateVolumeTextureFromFileExA(LPDIRECT3DDEVICE9 pDevice,
						LPCTSTR pSrcFile,
						UINT Width,
						UINT Height,
						UINT Depth,
						UINT MipLevels,
						DWORD Usage,
						D3DFORMAT Format,
						D3DPOOL Pool,
						DWORD Filter,
						DWORD MipFilter,
						D3DCOLOR ColorKey,
						D3DXIMAGE_INFO *pSrcInfo,
						PALETTEENTRY *pPalette,
						LPDIRECT3DTEXTURE9 *ppVolumeTexture);
HRESULT FAR WINAPI OBGECreateVolumeTextureFromFileInMemory(LPDIRECT3DDEVICE9 pDevice,
						      LPCVOID pSrcData,
						      UINT SrcDataSize,
						      LPDIRECT3DTEXTURE9 *ppVolumeTexture);
HRESULT FAR WINAPI OBGECreateVolumeTextureFromFileInMemoryEx(LPDIRECT3DDEVICE9 pDevice,
							LPCVOID pSrcData,
							UINT SrcDataSize,
							UINT Width,
							UINT Height,
							UINT Depth,
							UINT MipLevels,
							DWORD Usage,
							D3DFORMAT Format,
							D3DPOOL Pool,
							DWORD Filter,
							DWORD MipFilter,
							D3DCOLOR ColorKey,
							D3DXIMAGE_INFO *pSrcInfo,
							PALETTEENTRY *pPalette,
							LPDIRECT3DTEXTURE9 *ppVolumeTexture);

#endif
