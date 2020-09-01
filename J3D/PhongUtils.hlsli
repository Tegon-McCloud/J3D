
#define PHONG

#define DIF_SLOT 0
#define SPC_SLOT 1
#define NRM_SLOT 2

#ifndef __cplusplus

#define CONCAT(x, y) x##y

#ifdef DIF_MAP
Texture2D difTex : register(CONCAT(t, DIF_SLOT));



#endif