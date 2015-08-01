//源文件实现对应的函数体 0空指针
#include "Utility.h"

bool d3d::IntD3D(HINSTANCE hinstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	WNDCLASS wc;    //窗口的结构体，然后实现相应的字段
	static TCHAR szAppName[] = TEXT("Direct3DApp");				//新版本的用text转换一下
	wc.style = CS_HREDRAW | CS_VREDRAW;							//设置窗口的格式
	wc.lpfnWndProc = d3d::WndPro;								//设置窗口的回调
	wc.cbClsExtra = 0;											//
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hCursor = LoadCursor(0,IDC_ARROW);					//鼠标
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);				//图标
	wc.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));	//用画刷将背景刷成白色
	wc.lpszMenuName = 0;									//菜单名字				
	wc.lpszClassName = szAppName;								//窗口名字

	if (!RegisterClass(&wc))									//窗口注册
	{
		//如果注册不成功则显示提示框
		::MessageBox(0, TEXT("RegisterClass() -- Failed"), 0, 0);
		return false;
	}

	//创建窗口
	HWND hwnd = 0;
	hwnd = ::CreateWindow(szAppName, TEXT("Direct3DApp"), WS_EX_TOPMOST, 0, 0, width, height, 0, 0, hinstance, 0);
	if (!hwnd)
	{
		::MessageBox(0, TEXT("CreateWindow() -- Failed"), 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);								//显示窗口
	::UpdateWindow(hwnd);										//更新窗口

	//创建Direct设备
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;									//声明一个指针
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);					//获取D3D的版本

	if (!d3d9)
	{
		::MessageBox(0, TEXT("Direct3DCreate9() -- Failed"), 0, 0);
		return false;
	}

	//获取设备参数
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	
	//判断是用软件绘制还是硬件绘制
	int vp = 0;													//存储绘制的信息
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)			//判断设备是否支持硬件绘制
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//填充一个结构体，用于后台缓存
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//创建设备
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

	//如果创建失败，则换用低版本的设备
	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;   //将深度格式设为D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);
		if (FAILED(hr))
		{
			//说明显卡太差，释放掉
			d3d9->Release();
			::MessageBox(0, TEXT("DirectDirect() -- Failed"), 0, 0);
			return false;
		}
		d3d9->Release();

	}

	return true;
}

//用于消息循环
int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;

	//开辟一快内存
	::ZeroMemory(&msg, sizeof(MSG));
	static float lastTime = (float)timeGetTime();   //获得当前时间

	//发送消息
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);   //转换消息
			::DispatchMessage(&msg);    //发送消息

		}
		else
		{
			float currentTime = (float)timeGetTime();
			float timeDelta = (currentTime - lastTime) * 0.001f;
			ptr_display(timeDelta);
			lastTime = currentTime;
		}
	}
	return msg.wParam;
}

//初始化点光源，平行光和聚光灯
D3DLIGHT9 d3d::InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));	//开辟内存空调

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Ambient = *color * 0.6f;			
	light.Diffuse = *color; //本身的颜色
	light.Specular = *color * 0.6f; //高光
	light.Direction = *direction;
	return light;
}
D3DLIGHT9 d3d::InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));	//开辟内存空调

	light.Type = D3DLIGHT_POINT;
	light.Ambient = *color * 0.6f;		//周围
	light.Diffuse = *color; //本身的颜色
	light.Specular = *color * 0.6f; //高光
	light.Position = *position;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;  //衰减系数，只用于聚光灯
	light.Attenuation0 = 1.0f;//衰减系数
	light.Attenuation1 = 1.0f;//衰减系数
	light.Attenuation2 = 0.0f;//衰减系数

	return light;
}
D3DLIGHT9 d3d::InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));	//开辟内存空调

	light.Type = D3DLIGHT_SPOT;
	light.Ambient = *color * 0.0f;		//周围
	light.Diffuse = *color; //本身的颜色
	light.Specular = *color * 0.6f; //高光
	light.Position = *position;
	light.Direction = *direction;
	light.Range = 1000.0f;
	light.Falloff = 1.0f;  //衰减系数
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta = 0.4f;   
	light.Phi = 0.9f;
	return light;
}

//初始化材质
D3DMATERIAL9 d3d::InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient = a;
	mtrl.Diffuse = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power = p;
	return mtrl;
}

