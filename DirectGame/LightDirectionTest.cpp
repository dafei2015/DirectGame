//初始化一个方向光
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //全局的设备指针

const int width = 640;
const int height = 480;


ID3DXMesh* objects[4] = { 0, 0, 0, 0 };	//模型
D3DXMATRIX worlds[4];					//坐标
D3DMATERIAL9 mtrls[4];					//材质


//开始设置
bool SetUp()
{
	//创建对象
	D3DXCreateTeapot(device, &objects[0], 0);  //茶壶
	D3DXCreateSphere(device, 1.0f, 20, 20, &objects[1], 0);
	D3DXCreateTorus(device, 0.5f, 1.0f, 20, 20, &objects[2], 0);
	D3DXCreateCylinder(device, 0.5f, 0.5f, 2.0f, 20, 20, &objects[3], 0);

	//取出世界矩阵，然后将对象放进去,只有在世界矩阵中才会显示
	D3DXMatrixTranslation(&worlds[0], 0.0f, 2.0f, 0.0f);
	D3DXMatrixTranslation(&worlds[1], 0.0f, -2.0f, 0.0f); 
	D3DXMatrixTranslation(&worlds[2], -3.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&worlds[3], 3.0f, 2.0f, 0.0f);

	//设置对象的材质
	mtrls[0] = d3d::RED_MTRL;
	mtrls[1] = d3d::BLUE_MTRL;
	mtrls[2] = d3d::GREEN_MTRL;
	mtrls[3] = d3d::RED_MTRL;

	//设置方向光
	D3DXVECTOR3 dir(1.0f, -0.0f, 0.25f);
	D3DXCOLOR c = d3d::WHITE;
	D3DLIGHT9 dirlight = d3d::InitDirectionalLight(&dir, &c);

	//设置设备的光线
	device->SetLight(0, &dirlight);
	device->LightEnable(0, true);  //启动光线


	//设置渲染,开启法线
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true); //开启法线
	device->SetRenderState(D3DRS_SPECULARENABLE, false);//关闭高光

	//设置投影矩阵
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)width / (float)height, 1.0f, 1000.0f);

	//将其投影到屏幕上
	device->SetTransform(D3DTS_PROJECTION, &proj);
	return true;
}

//释放
void Cleanup()
{
	for (int i = 0; i < 4; i++)
	{
		d3d::Release<ID3DXMesh*>(objects[i]);
	}
}

bool Display(float timeDelta)
{
	//如果设备存在 先清空，在绘制
	if (device)
	{
		//通过摄像机旋转显示各个面
		static float angle = (3.0f*D3DX_PI) / 2.0f;  //定义一个旋转角度
		static float height = 5.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f) //判断键盘的输入方向
			angle -= 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f) //判断键盘的输入方向
			angle += 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_UP) & 0x8000f) //判断键盘的输入方向
			height -= 0.5f * timeDelta;
		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f) //判断键盘的输入方向
			height += 0.5f * timeDelta;

		//摄像机移动
		D3DXVECTOR3 position(cosf(angle)*7.0f, height, sinf(angle)*7.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

		//定义一个世界矩阵，将相机放入
		D3DXMATRIX v;
		D3DXMatrixLookAtLH(&v, &position, &target, &up);  //定义为左手坐标系
		device->SetTransform(D3DTS_VIEW, &v);                     //将其放在视口矩阵里

		//开始画场景
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x0000000, 1.0f, 0);
		device->BeginScene();
		
		for (int i = 0; i < 4; i++)
		{
			device->SetMaterial(&mtrls[i]);
			device->SetTransform(D3DTS_WORLD, &worlds[i]);
			objects[i]->DrawSubset(0);
		}
		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}

//窗口的回调
LRESULT CALLBACK d3d::WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}

	return ::DefWindowProcA(hwnd, msg, wParam, lParam);
}

//主函数
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	if (!d3d::IntD3D(hInstance, 640, 480, true, D3DDEVTYPE_HAL, &device))
	{
		::MessageBox(0, TEXT("InitD3D()-- failed"), 0, 0);
		return 0;
	}

	if (!SetUp())
	{
		::MessageBox(0, TEXT("SetUp()-- failed"), 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();
	device->Release();
	return 0;
}
