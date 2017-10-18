#pragma once

#include "ClassQueue.h"

//- 본 클래스 내부에서 자식 윈도우의 생성, 자식 윈도우의 프로시저, 데이터 등을 모두 가진다.


//// 자식 윈도우 클래스 헤더 ////////////////////////////

#define dfMAXCHILD		100
#define UM_ALERT		WM_USER + 1

// SendMessage(부모핸들, UM_ALERT...) 를 통해서 부모에게 경고를 알린다
// 부모는 경고 메시지를 받은경우 배경을 빨간색으로 칠함.
// 일정시간(300~500 ms)이후 정상으로 돌아오고 경고는 꺼짐.

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
	// 그래프 정보 입력
	//
	// 윈도우 이름, 최대치, 경보수치 (0이면 없음)
	/////////////////////////////////////////////////////////
	void	SetInformation(WCHAR *szTitle, int iDataMax, int iDataAlert);

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
	/////////////////////////////////////////////////////////////
	// 경고수치 알람발생.  InsertData 에서 호출되는 함수
	/////////////////////////////////////////////////////////////
	BOOL	Alert(void);

	/////////////////////////////////////////////////////////////
	// 타입별 Paint 함수
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
	// 메모리DC 화면 지우기
	////////////////////////////////////////////////////////////
	void	ClearMemDC(void);

	/////////////////////////////////////////////////////////////
	// 메모리DC 화면으로 전환
	////////////////////////////////////////////////////////////
	void	FlipMemDC(HDC hDC);

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
	HFONT _font;

	WCHAR _szWindowClass[30];
	int _iWindowPosX;
	int _iWindowPosY;
	int _iWindowWidth;
	int _iWindowHeight;

	//------------------------------------------------------
	// 정보창 이름
	//------------------------------------------------------
	WCHAR _szTitle[32];

	//------------------------------------------------------
	// 데이터, 최고치, 경보 수치
	//------------------------------------------------------
	int		_iDataMax;	// 그래프 맥스 값
	int		_iDataAlert;	// 경보 울리는 기준값
	
	Queue<int>* _dataQ;

	HDC _memDC;
	HBITMAP _bitmap;
	HBITMAP _oldBitmap;
	RECT _rect;

	// static 맴버 함수의 프로시저에서 This 포인터를 찾기 위한
	// HWND + Class Ptr 의 테이블
	static ST_HWNDtoTHIS _childInfoTable[dfMAXCHILD];

	//------------------------------------------------------
	// 경고모드 - 
	//
	// 데이터 경고수치 도달시 경고음과 함께 자신을 제외한 모든 윈도우에 UM_ALERT 메시지를 보낸다.
	//
	// _bAlertMode = TRUE 가 된 후 타이머를 사용하여 일정시간 후 해제.
	//------------------------------------------------------
	BOOL _bAlertMode;
};