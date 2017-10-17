#pragma once

#include "ClassQueue.h"

//- 본 클래스 내부에서 자식 윈도우의 생성, 자식 윈도우의 프로시저, 데이터 등을 모두 가진다.


//// 자식 윈도우 클래스 헤더 ////////////////////////////

#define dfMAXCHILD		100

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
	// 윈도우 프로시저
	/////////////////////////////////////////////////////////
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	/////////////////////////////////////////////////////////
	// 데이터 넣기.
	/////////////////////////////////////////////////////////
	BOOL	InsertData(int iData);
	void Init();
	void DrawInit();
	void WindowInit();

protected:

	//------------------------------------------------------
	// 윈도우 핸들, this 포인터 매칭 테이블 관리.
	//------------------------------------------------------
	BOOL				PutThis(void);
	//static BOOL			RemoveThis(HWND hWnd);
	static CMonitorGraphUnit	*GetThis(HWND hWnd);

private:

	//------------------------------------------------------
	// 부모 윈도우 핸들, 내 윈도우 핸들, 인스턴스 핸들
	//------------------------------------------------------
	HWND _hWndParent;
	HWND _hWnd;
	HINSTANCE _hInstance;

	//------------------------------------------------------
	// 윈도우 위치,크기,색상, 그래프 타입 등.. 자료
	//------------------------------------------------------
	TYPE _enGraphType;
	Color _backColor;

	HBRUSH _brushArr[5];
	HPEN _penArr[5];

	WCHAR _szWindowClass[30] = L"";
	int _iWindowPosX;
	int _iWindowPosY;
	int _iWindowWidth;
	int _iWindowHeight;


	//------------------------------------------------------
	// 데이터
	//------------------------------------------------------
	//데이터 큐
	
	Queue<int>* _dataQ;

	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _oldBitmap;
	RECT _rect;

	// static 맴버 함수의 프로시저에서 This 포인터를 찾기 위한
	// HWND + Class Ptr 의 테이블
	static ST_HWNDtoTHIS _childInfoTable[dfMAXCHILD];
};