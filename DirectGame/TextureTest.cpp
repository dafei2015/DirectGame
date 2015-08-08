//初始化一个带纹理的四边形
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //全局的设备指针

const int width = 640;
const int height = 480;

//声明一个四边形的顶点结构
IDirect3DVertexBuffer9* quad = 0;
//声明一个纹理
IDirect3DTexture9* tex = 0;

//声明一个结构体
struct Vertex
{
	//三维点
	float _x, _y, _z;

	//法线
	float _nx, _ny, _nz;

	//UV坐标
	float _u, _v;

	Vertex()
	{
	};


	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v)
		: _x(x),
		  _y(y),
		  _z(z),
		  _nx(nx),
		  _ny(ny),
		  _nz(nz),
		  _u(u),
		  _v(v)
	{
		_x = x; _y = y; _z = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u = u; _v = v;
	}

	static const DWORD FVF;
};

//设置顶点的格式.如果没有法线则不能显示画面
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

//开始设置
bool SetUp()
{
	//创建对象，一个四边形含有两个三角形，6个顶点
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &quad, 0);

	Vertex* v;
	quad->Lock(0, 0, (void**)&v, 0);
	//定义一个三角形
	v[0] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	//定义第二个三角形
	v[3] = Vertex(-1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(1.0f, 1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(1.0f, -1.0f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	quad->Unlock();

	//加载图片
	D3DXCreateTextureFromFile(device, "test.jpg", &tex);
	device->SetTexture(0, tex);

	//设置采样模式为线性过滤
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//设置渲染,将灯光关掉
	device->SetRenderState(D3DRS_LIGHTING, false);
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
	d3d::Release<IDirect3DVertexBuffer9*>(quad);
	d3d::Release<IDirect3DTexture9*>(tex);
}

bool Display(float timeDelta)
{
	//如果设备存在 先清空，在绘制
	if (device)
	{
		//开始画场景
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfffffff, 1.0f, 0);
		device->BeginScene();

		//设置流文件
		device->SetStreamSource(0, quad, 0, sizeof(Vertex));
		//设置顶点格式
		device->SetFVF(Vertex::FVF);
		//画三角形
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

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
