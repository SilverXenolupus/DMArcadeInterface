#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#define _CRT_SECURE_NO_WARNINGS

#include <math.h>

#include "d3dclass.h"
#include "debugoutput.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"
#include "GameInfo.h"
#include "transparentshaderclass.h"
#include "textureshaderclass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(string[], string[], int, int[], int, int, float, float, float, int&);
	bool Render(float[], float[], float[], int, float, float, float);

private:
	D3DClass* m_D3D;
	DebugOutput* m_Log;
	CameraClass* m_Camera;
	ModelClass* m_Models[8];
	//ModelClass* m_Textures[];
	ModelClass* m_DMModel;
	ModelClass* m_FadeModel;
	ModelClass* m_ChooseModel;
	ModelClass* m_ArrowL;
	ModelClass* m_ArrowR;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;
	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	TextClass* m_Text;
	TransparentShaderClass* m_TransparentShader;

	float turnAmount = 0;
	float x = 0.01f;
	float zoom = 0;
	float fade = 1;
};

#endif