#pragma once
#include<time.h>    // clock_t   ����� ���� ����
typedef class Timer
{
public:
    Timer();
    void   reset();
    void   updateLogic();
    double getTime();
    double getUpdateCount();
    double getRenderFPS();
    double getDebugFPS();
    double getLPS();
    bool   isGoingUpdate();
//    void   render(const HDC hDC, int x, int y);
//    void   debug(const HDC hDC, int x, int y);
protected:
    // ���۽ð�, �����ð�
    clock_t _start, _end;
    // ����ð� = �����ð� - ���۽ð�
    double _time;
    double _renderCount;
    double _debugCount;
    double _updateCount;
    const static int MAX_UPDATE_PER_SEC = 100;
}Timer;