//材质融合，实现将一个物体和一个图片进行融合，利用阿尔法通道融合技术
#include "Utility.h"

IDirect3DDevice9* device = nullptr; //全局的设备指针

const int width = 640;
const int height = 480;

//声明一个四边形的顶点结构
IDirect3DVertexBuffer9* BGQuad = 0;
//声明一个纹理
IDirect3DTexture9* BGTex = 0;
//融合的是将材质进行融合
D3DMATERIAL9 BGMtr;

//需要融合的mesh 和材质
ID3DXMesh* teapot = 0;
D3DMATERIAL9 teapotMtr;

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
	}
	static const DWORD FVF;
};

//设置顶点的格式.如果没有法线则不能显示画面
const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_NORMAL;

//开始设置
bool SetUp()
{
	//创建物体材质和背景材质
	teapotMtr = d3d::RED_MTRL;
	teapotMtr.Diffuse.a = 0.5f;
	BGMtr = d3d::WHITE_MTRL;

	//创建该物体
	D3DXCreateTeapot(device, &teapot, 0);  //茶壶

	//创建对象，一个四边形含有两个三角形，6个顶点
	device->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &BGQuad, 0);

	Vertex* v;
	BGQuad->Lock(0, 0, (void**)&v, 0);
	//定义一个三角形
	v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	//定义第二个三角形
	v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	BGQuad->Unlock();

	//创建一个方向光
	D3DLIGHT9 dirlight;
	::ZeroMemory(&dirlight, sizeof(dirlight));
	dirlight.Type = D3DLIGHT_DIRECTIONAL;
	dirlight.Diffuse = d3d::WHITE;
	dirlight.Specular = d3d::WHITE * 0.2f;
	dirlight.Ambient = d3d::WHITE * 0.6f;
	dirlight.Direction = D3DXVECTOR3(0.707f, 0.0, 0.707f);

	//设置设备的光线
	device->SetLight(0, &dirlight);
	device->LightEnable(0, true);  //启动光线
	
	//设置渲染状态
	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);//设置法线
	device->SetRenderState(D3DRS_SPECULARENABLE, true); //设置高光


	//加载图片
	D3DXCreateTextureFromFile(device, "test.jpg", &BGTex);
	device->SetTexture(0, BGTex);

	//设置采样模式为线性过滤
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//将两种颜色混合
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//设置混合因子
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//设置摄像机
	D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	//定义一个世界矩阵，将相机放入
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);			//设定相机的位置

	device->SetTransform(D3DTS_VIEW, &view);						//将其放在视口矩阵里
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
	d3d::Release<IDirect3DVertexBuffer9*>(BGQuad);
	d3d::Release<IDirect3DTexture9*>(BGTex);
	d3d::Release<ID3DXMesh*>(teapot);

}

bool Display(float timeDelta)
{
	//如果设备存在 先清空，在绘制
	if (device)
	{
		//使用键盘控制透明度
		if (::GetAsyncKeyState('A') & 0x8000f) //判断键盘的输入方向
			teapotMtr.Diffuse.a += 0.0001f;
		if (::GetAsyncKeyState('D') & 0x8000f) //判断键盘的输入方向
			teapotMtr.Diffuse.a -= 0.0001f;

		if (teapotMtr.Diffuse.a > 1.0f)
			teapotMtr.Diffuse.a = 1.0f;
		if (teapotMtr.Diffuse.a < 0.0f)
			teapotMtr.Diffuse.a = 0.0f;
		//开始画场景
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xfffffff, 1.0f, 0);
		device->BeginScene();

		//将背景放在世界坐标戏中
		D3DXMATRIX w;
		D3DXMatrixIdentity(&w);
		device->SetTransform(D3DTS_WORLD, &w);
		device->SetFVF(Vertex::FVF);

		device->SetStreamSource(0, BGQuad, 0, sizeof(Vertex));
		device->SetMaterial(&BGMtr);
		device->SetTexture(0,BGTex);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		//画茶壶
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		D3DXMatrixScaling(&w, 1.5f, 1.5f, 1.5f);//将物体放大，并放到世界矩阵中
		device->SetTransform(D3DTS_WORLD, &w);
		device->SetMaterial(&teapotMtr);
		device->SetTexture(0, 0);
		teapot->DrawSubset(0);   //调用物体的相应接口绘制该物体

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
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
