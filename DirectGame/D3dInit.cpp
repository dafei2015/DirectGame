#include "Utility.h"

IDirect3DDevice9* device = nullptr;   //全局的设备指针


//着色，定义个世界坐标矩阵
D3DXMATRIX worldMatrix;

//定义一个顶点缓存
IDirect3DVertexBuffer9* triangle = 0;

//写一个顶点颜色的结构体
struct ColorVertex
{
	float _x, _y, _z;
	D3DCOLOR _color;
	ColorVertex(){	}


	ColorVertex(float x, float y, float z, D3DCOLOR d3_dcolor)
		: _x(x),
		  _y(y),
		  _z(z),
		  _color(d3_dcolor)
	{
		_x = x;
		_y = y;
		_z = z;
		_color = d3_dcolor;
	}

	static const DWORD FVF;
};

//顶点形式D3DFVF_XYZ 表示xyz坐标，D3DFVF_DIFFUSE表示rgb值
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;

const int width = 640;
const int height = 480;

//开始设置
bool SetUp()
{
	//创建 一个顶点缓存
	device->CreateVertexBuffer(
		8 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &triangle, 0
		);

	ColorVertex* v;
	triangle->Lock(0, 0, (void**)&v, 0);
	//定义一个三角形
	v[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = ColorVertex(-1.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
	v[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

	triangle->Unlock();

	//设置投影矩阵
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, (float)width / (float)height, 1.0f, 1000.0f);

	//将其投影到屏幕上
	device->SetTransform(D3DTS_PROJECTION, &proj);

	//设置渲染,将灯光关掉
	device->SetRenderState(D3DRS_LIGHTING,false);
	return true;
}

//释放
void Cleanup()
{
	d3d::Release<IDirect3DVertexBuffer9*>(triangle);
}

bool Display(float timeDelta)
{
	//如果设备存在 先清空，在绘制
	if (device)
	{
		//先将画布清空，然后开始画画布
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfffffff, 1.0f, 0);
		device->BeginScene();
		//设置流文件
		device->SetStreamSource(0, triangle, 0, sizeof(ColorVertex));
		//设置顶点格式
		device->SetFVF(ColorVertex::FVF);

		//画一个三角形
		D3DXMatrixTranslation(&worldMatrix, -1.25f, 0.0f, 0.0f);

		//将其放置在世界矩阵中
		device->SetTransform(D3DTS_WORLD, &worldMatrix);

		//设置渲染模式，平滑渲染
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);

		//画三角形
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		//画第二个三角形
		D3DXMatrixTranslation(&worldMatrix, 1.25f, 0.0f, 0.0f);

		//将其放置在世界矩阵中
		device->SetTransform(D3DTS_WORLD, &worldMatrix);

		//设置渲染模式，平滑渲染
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);

		//画三角形
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
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

