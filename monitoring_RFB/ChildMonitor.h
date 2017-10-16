#pragma once

#include "ClassQueue.h"

//- �� Ŭ���� ���ο��� �ڽ� �������� ����, �ڽ� �������� ���ν���, ������ ���� ��� ������.


//// �ڽ� ������ Ŭ���� ��� ////////////////////////////

#define dfMAXCHILD		100

class CMonitorGraphUnit
{
public:

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
		CMonitorGraphUnit	*pThis[dfMAXCHILD];

	};

public:

	CMonitorGraphUnit(HINSTANCE hInstance, HWND hWndParent, COLORREF BackColor, TYPE enType, int iPosX, int iPosY, int iWidth, int iHeight);
	~CMonitorGraphUnit();

	/////////////////////////////////////////////////////////
	// ������ ���ν���
	/////////////////////////////////////////////////////////
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////////
	// ������ �ֱ�.
	/////////////////////////////////////////////////////////
	BOOL	InsertData(int iData);

protected:

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
	TYPE		_enGraphType;
	COLORREF _backColor;

	WCHAR _szWindowClass[30] = L"";
	int _iWindowPosX;
	int _iWindowPosY;
	int _iWindowWidth;
	int _iWindowHeight;


	//------------------------------------------------------
	// ������
	//------------------------------------------------------
	//������ ť
	Queue<int>* _dataQ;

};