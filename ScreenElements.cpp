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

#include "ScreenElements.h"
#include "TextureManager.h"
#include "Rendering.h"

void	ScreenElement::SetPosition(float x,float y, float z)
{
  pos.x=x;
  pos.y=y;
  pos.z=z;
}

void	ScreenElement::SetRotation(float rotation)
{
  rot=rotation;
  UpdateTransform();
}

void	ScreenElement::SetScale(float x, float y)
{
  scale.x=x;
  scale.y=y;
  UpdateTransform();
}

void	ScreenElement::UpdateTransform(void)
{
  D3DXMATRIX TempScale, TempRot;
  D3DXMatrixScaling(&TempScale, scale.x, scale.y, 1);
  D3DXMatrixRotationZ(&TempRot, rot);
  transform=TempScale*TempRot;
}

void	ScreenElement::SetColor(float red, float green, float blue)
{
  if(red<0||red>1||green<0||green>1||blue<0||blue>1) {
    _MESSAGE("Tried to set a hud element to an invalid colour");
    return;
  }
  float alpha=((D3DXCOLOR)color).a;
  color=(DWORD)D3DXCOLOR(red,green,blue,alpha);
}

void	ScreenElement::SetAlpha(float alpha)
{
  if (alpha<0||alpha>1)
  {
    _MESSAGE("Tried to set a hud element to an invalid colour");
    return;
  }

  float red=((D3DXCOLOR)color).r;
  float green=((D3DXCOLOR)color).g;
  float blue=((D3DXCOLOR)color).b;
  color=(DWORD)D3DXCOLOR(red,green,blue,alpha);
}

bool Sprite::SetTexture(int texture) {
  tex = texture;
  return true;
}

int Sprite::GetTexture() {
  return tex;
}

void Sprite::Render(ID3DXSprite *sprite) {
  TextureManager *TexMan = TextureManager::GetSingleton();

  if (enabled && TexMan->IsTextureValid(tex)) {
    TextureRecord *TexRec = TexMan->GetTexture(tex);

    if (TexRec->IsType(TR_PLANAR)) {
      sprite->SetTransform(&transform);
      sprite->Draw((LPDIRECT3DTEXTURE9)TexRec->GetTexture(), 0, 0, &pos, color);
    }
  }
}

HUDManager::HUDManager() {
  NextElementIndex = 0;
  sprite = NULL;
  Singleton = this;

}

HUDManager *HUDManager::Singleton = NULL;

HUDManager *HUDManager::GetSingleton() {
  if (Singleton)
    return Singleton;
  else
    return new HUDManager();
}

int HUDManager::AddScreenElement(Sprite *data) {
  ScreenElementList.push_back(data);

  return ++NextElementIndex;
}

Sprite *HUDManager::index(int ind) {
  return ScreenElementList[ind];
}

void HUDManager::PurgeTexture(IDirect3DBaseTexture9 *texture, int TexNum) {
  /* release previous texture */
  std::vector<Sprite *>::iterator SE = ScreenElementList.begin();

  while (SE != ScreenElementList.end()) {
    if ((*SE)->GetTexture() == TexNum) {
      (*SE)->SetTexture(-1);
//    (*SE)->enabled = false;
    }

    SE++;
  }
}

void HUDManager::Render(IDirect3DDevice9 *Device) {
  HRESULT hr;

  if (!Device) {
    _MESSAGE("No valid D3D Device.");
    return;
  }

  if(!sprite) {
    hr = D3DXCreateSprite(Device, &sprite);
    if (FAILED(hr) || !sprite) {
      _MESSAGE("Create sprite failed.");
      return;
    }
  }

  sprite->Begin(0);

  for (std::vector<Sprite *>::iterator i = ScreenElementList.begin(); i != ScreenElementList.end(); i++)
    (*i)->Render(sprite);

  sprite->End();
  return;
}

void HUDManager::DeviceLost() {
  if (sprite)
    sprite->OnLostDevice();
}

void HUDManager::DeviceReset() {
  if (sprite)
    sprite->OnResetDevice();
}
