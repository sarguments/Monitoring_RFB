#pragma once

#include "ClassQueue.h"

//- �� Ŭ���� ���ο��� �ڽ� �������� ����, �ڽ� �������� ���ν���, ������ ���� ��� ������.


//// �ڽ� ������ Ŭ���� ��� ////////////////////////////

#define dfMAXCHILD		100
#define UM_ALERT		WM_USER + 1

// SendMessage(�θ��ڵ�, UM_ALERT...) �� ���ؼ� �θ𿡰� ��� �˸���
// �θ�� ��� �޽����� ������� ����� ���������� ĥ��.
// �����ð�(300~500 ms)���� �������� ���ƿ��� ���� ����.

class CMonitorGraphUnit
{
public:
	enum Color
	{
		RED,
		GREEN,
		BLUE,
		YELLOW,
		PINK
	};

	enum TYPE
	{
		BAR_SINGLE_VERT,
		BAR_COLUMN_VERT,
		LINE_SINGLE,
		LINE_MULTI,
		PIE
	};

	struct ST_HWNDtoTHIS
	{
		HWND				hWnd[dfMAXCHILD];
		CMonitorGraphUnit* pThis[dfMAXCHILD];

	};

public:
	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, Color color, TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight);
	~CMonitorGraphUnit();

	/////////////////////////////////////////////////////////
	// �׷��� ���� �Է�
	//
	// ������ �̸�, �ִ�ġ, �溸��ġ (0�̸� ����)
	/////////////////////////////////////////////////////////
	void	SetInformation(WCHAR *szTitle, int iDataMax, int iDataAlert);

	/////////////////////////////////////////////////////////
	// ������ ���ν���
	/////////////////////////////////////////////////////////
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////////
	// ������ �ֱ�.
	/////////////////////////////////////////////////////////
	BOOL	InsertData(int iData);
	void Init();
	void DrawInit();
	void WindowInit();

protected:
	/////////////////////////////////////////////////////////////
	// ����ġ �˶��߻�.  InsertData ���� ȣ��Ǵ� �Լ�
	/////////////////////////////////////////////////////////////
	BOOL	Alert(void);

	/////////////////////////////////////////////////////////////
	// Ÿ�Ժ� Paint �Լ�
	/////////////////////////////////////////////////////////////
	//void	Paint_Title(void);
	void	Paint_Grid(void);


	void	Paint_BarSingleVert(void);
	void	Paint_BarSingleHorz(void);
	void	Paint_BarColumnVert(void);
	void	Paint_BarColumnHorz(void);
	void	Paint_LineSingle(void);
	void	Paint_LineMulti(void);
	void	Paint_Pie(void);

	////////////////////////////////////////////////////////////
	// �޸�DC ȭ�� �����
	////////////////////////////////////////////////////////////
	void	ClearMemDC(void);

	/////////////////////////////////////////////////////////////
	// �޸�DC ȭ������ ��ȯ
	////////////////////////////////////////////////////////////
	void	FlipMemDC(HDC hDC);

	//------------------------------------------------------
	// ������ �ڵ�, this ������ ��Ī ���̺� ����.
	//------------------------------------------------------
	BOOL				PutThis(void);
	//static BOOL			RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

private:

	//------------------------------------------------------
	// �θ� ������ �ڵ�, �� ������ �ڵ�, �ν��Ͻ� �ڵ�
	//------------------------------------------------------
	HWND _hWndParent;
	HWND _hWnd;
	HINSTANCE _hInstance;

	//------------------------------------------------------
	// ������ ��ġ,ũ��,����, �׷��� Ÿ�� ��.. �ڷ�
	//------------------------------------------------------
	TYPE _enGraphType;
	Color _backColor;

	HBRUSH _brushArr[5];
	HPEN _penArr[5];
	HFONT _font;

	WCHAR _szWindowClass[30];
	int _iWindowPosX;
	int _iWindowPosY;
	int _iWindowWidth;
	int _iWindowHeight;

	//------------------------------------------------------
	// ����â �̸�
	//------------------------------------------------------
	WCHAR _szTitle[32];

	//------------------------------------------------------
	// ������, �ְ�ġ, �溸 ��ġ
	//------------------------------------------------------
	int		_iDataMax;	// �׷��� �ƽ� ��
	int		_iDataAlert;	// �溸 �︮�� ���ذ�
	
	Queue<int>* _dataQ;

	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _oldBitmap;
	RECT _rect;

	// static �ɹ� �Լ��� ���ν������� This �����͸� ã�� ����
	// HWND + Class Ptr �� ���̺�
	static ST_HWNDtoTHIS _childInfoTable[dfMAXCHILD];

	//------------------------------------------------------
	// ����� - 
	//
	// ������ ����ġ ���޽� ������� �Բ� �ڽ��� ������ ��� �����쿡 UM_ALERT �޽����� ������.
	//
	// _bAlertMode = TRUE �� �� �� Ÿ�̸Ӹ� ����Ͽ� �����ð� �� ����.
	//------------------------------------------------------
	BOOL _bAlertMode;
};