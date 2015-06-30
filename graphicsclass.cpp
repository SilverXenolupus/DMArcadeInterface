#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	for (int i = 0; i < 8; i++)
	{
		m_Models[i] = 0;
	}
	m_DMModel = 0;
	m_FadeModel = 0;
	m_ChooseModel = 0;
	m_ArrowL = 0;
	m_ArrowR = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_Text = 0;
	m_TransparentShader = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	D3DXMATRIX baseViewMatrix;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}
	m_Log = new DebugOutput;
	if (!m_Log)
	{
		return false;
	}

	//m_D3D->GetVideoCardInfo(m_Log->VideocardName, m_Log->Memory);
	//5m_Log->OutputVideoInfo();

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.f, -0.4f, -12.f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	for (int i = 0; i < 8; i++)
	{
		// Create the model object.
		m_Models[i] = new ModelClass;
		if (!m_Models[i])
		{
			return false;
		}
		// Initialize the model object.
		result = m_Models[i]->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/thumb.txt", L"../DMArcadeInterface/data/soft.dds");
		if (!result)
		{
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	// Create the model object.
	m_DMModel = new ModelClass;
	if (!m_DMModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_DMModel->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/dmlogo.txt", L"../DMArcadeInterface/data/DMLogo.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_FadeModel = new ModelClass;
	if (!m_FadeModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_FadeModel->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/fade.txt", L"../DMArcadeInterface/data/black.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_ChooseModel = new ModelClass;
	if (!m_ChooseModel)
	{
		return false;
	}
	// Initialize the model object.
	result = m_ChooseModel->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/choose.txt", L"../DMArcadeInterface/data/choose.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_ArrowL = new ModelClass;
	if (!m_ArrowL)
	{
		return false;
	}
	// Initialize the model object.
	result = m_ArrowL->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/arrowL.txt", L"../DMArcadeInterface/data/arrow.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the model object.
	m_ArrowR = new ModelClass;
	if (!m_ArrowR)
	{
		return false;
	}
	// Initialize the model object.
	result = m_ArrowR->Initialize(m_D3D->GetDevice(), "../DMArcadeInterface/data/arrowR.txt", L"../DMArcadeInterface/data/arrow.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}
	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.5f, 0.5f, 0.5f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.5f, 1.0f);
	m_Light->SetSpecularColor(0.6f, 0.6f, 0.6f, 0.2f);
	m_Light->SetSpecularPower(4.0f);

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), 2120, 1280, L"../DMArcadeInterface/data/background.dds", 1920, 1080);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the transparent shader object.
	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
	{
		return false;
	}

	// Initialize the transparent shader object.
	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the transparent shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	
	// Release the transparent shader object.
	if (m_TransparentShader)
	{
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TransparentShader = 0;
	}

	// Release the text object.
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}
	
	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if (m_ArrowR)
	{
		m_ArrowR->Shutdown();
		delete m_ArrowR;
		m_ArrowR = 0;
	}

	// Release the model object.
	if (m_ArrowL)
	{
		m_ArrowL->Shutdown();
		delete m_ArrowL;
		m_ArrowL = 0;
	}

	// Release the model object.
	if (m_ChooseModel)
	{
		m_ChooseModel->Shutdown();
		delete m_ChooseModel;
		m_ChooseModel = 0;
	}

	// Release the model object.
	if (m_FadeModel)
	{
		m_FadeModel->Shutdown();
		delete m_FadeModel;
		m_FadeModel = 0;
	}

	// Release the model object.
	if (m_DMModel)
	{
		m_DMModel->Shutdown();
		delete m_DMModel;
		m_DMModel = 0;
	}

	// Release the model object.
	for (int i = 0; i < 8; i++)
	{
		if (m_Models[i])
		{
			m_Models[i]->Shutdown();
			delete m_Models[i];
			m_Models[i] = 0;
		}
	}
	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}
	
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(string names[], string picDir[], int gameAmount, int defaultG[], int turn, int turnMin, float leftSpin, float rightSpin, float DMSpin, int& gameToOpen)
{

	bool result;

	float rotx[8];
	float roty[8];
	float rotz[8];

	turnAmount = (turnAmount + ((int)turn / 0.005f))/2;

	x += (float)D3DX_PI * 0.005f;

	zoom = x / x / x / x;

	fade = -1 / (pow(1 + exp(1.0f), -5 * x + 3)) + 1;
	// Update the rotation variable each frame.

	wchar_t picture[_MAX_DIR];

	for (int i = 0; i < 8; i++)
	{
		//if (i<5)
		int loopGame = defaultG[i];
		/*while (loopGame > gameAmount - 1)
		{
			loopGame -= gameAmount;
		}*/

		mbstowcs_s(0, picture, 256, picDir[loopGame].c_str(), 256);
		
		m_Models[i]->ReleaseTexture();
		m_Models[i]->LoadTexture(m_D3D->GetDevice(), picture);

		rotx[i] = (0.7862f * i) - turnAmount * 0.003925f;
		roty[i] = sin(x + i*0.1f + 0.5f)/32;
		rotz[i] = sin(x + i*0.6f)/32;

	}

	int loopName = defaultG[turnMin];

	result = m_Text->GetDirectories(names, loopName, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Render the graphics scene.
	result = Render(rotx, roty, rotz, turnMin, leftSpin, rightSpin, DMSpin);
	if (!result)
	{
		return false;
	}

	gameToOpen = loopName;

	return true;
}


bool GraphicsClass::Render(float rotx[], float roty[], float rotz[], int turnMin, float leftSpin, float rightSpin, float DMSpin)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->SetPosition(0.f, -0.5f, -14.f - zoom);
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	D3DXMatrixRotationY(&worldMatrix, 0);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
	for (int i = 0; i < 8; i++)
	{
		// Rotate the world matrix by the rotation value so that the triangle will spin.
		D3DXMatrixTranslation(&worldMatrix, 5, 0, 0);
		D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotx[i], roty[i], rotz[i]);
		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		m_Models[i]->Render(m_D3D->GetDeviceContext());
		// Render the model using the light shader.
		// Render the model using the light shader.
		result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
		if (!result)
		{
			return false;
		}
	}

	D3DXMatrixRotationYawPitchRoll(&worldMatrix, rotx[turnMin], roty[turnMin], rotz[turnMin]);
	// Turn on alpha blending for the transparency to work.
	m_D3D->TurnOnAlphaBlending();

	// Put the second square model on the graphics pipeline.
	m_ChooseModel->Render(m_D3D->GetDeviceContext());

	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_ChooseModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ChooseModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();

	m_Camera->SetPosition(0.f, 3.8f, -15.f - zoom);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);
	//D3DXMatrixTranslation(&worldMatrix, -6.5, -4.5, 2);
	D3DXMatrixRotationYawPitchRoll(&worldMatrix, roty[7] * 5 * DMSpin, roty[2] * 3 * -DMSpin, rotz[5] * 4 * DMSpin);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_DMModel->Render(m_D3D->GetDeviceContext());
	// Render the model using the light shader.
	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_DMModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_DMModel->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Camera->SetPosition(5.f, 1.5f, -10.f - zoom);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);
	//D3DXMatrixTranslation(&worldMatrix, -6.5, -4.5, 2);
	D3DXMatrixRotationYawPitchRoll(&worldMatrix, roty[0] * 4 + 0.5f + leftSpin, roty[5] * 1.5f, rotz[4] * 1.5f);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ArrowL->Render(m_D3D->GetDeviceContext());
	// Render the model using the light shader.
	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_ArrowL->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ArrowL->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	m_Camera->SetPosition(-5.f, 1.5f, -10.f - zoom);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);
	//D3DXMatrixTranslation(&worldMatrix, -6.5, -4.5, 2);
	D3DXMatrixRotationYawPitchRoll(&worldMatrix, roty[4] * 4 - 0.5f - rightSpin, roty[6] * 1.5f, rotz[1] * 1.5f);
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_ArrowR->Render(m_D3D->GetDeviceContext());
	// Render the model using the light shader.
	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_ArrowR->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_ArrowR->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
		m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	// Rotate the world matrix by the rotation value so that the triangle will spin.
	// Render the model using the light shader.
	// Render the model using the light shader.

	D3DXMatrixRotationY(&worldMatrix, 0);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	/*result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 100, 100);
	if (!result)
	{
		return false;
	}

	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
	if (!result)
	{
		return false;
	}*/

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Translate to the right by one unit and towards the camera by one unit.
	D3DXMatrixTranslation(&worldMatrix, 0.0, 0.0f, -10.0f);

	// Turn on alpha blending for the transparency to work.
	m_D3D->TurnOnAlphaBlending();

	// Put the second square model on the graphics pipeline.
	m_FadeModel->Render(m_D3D->GetDeviceContext());

	// Render the second square model with the stone texture and use the 50% blending amount for transparency.
	result = m_TransparentShader->Render(m_D3D->GetDeviceContext(), m_FadeModel->GetIndexCount(), worldMatrix, viewMatrix,
		projectionMatrix, m_FadeModel->GetTexture(), fade);
	if (!result)
	{
		return false;
	}

	// Turn off alpha blending.
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}