#pragma once
#include<Windows.h>
#include"Timer.h"
// 2d���ҽ��� �ν��Ͻ� �����ͷ� �޾ҵ��� 3d���ҽ��� �����ͷ� �޾Ƶ��� { _model = Sprite::getInstance(); _sound = SoundManager::getInstance(); }
typedef class Object
{
public:
    //�� �޼ҵ�
    Object();
    void   setPosition(double x, double y, double z) { _x = x; _y = y; _z = z; }
    bool   isPlayAble() { return _flagPlayAble; }
    void   setFlagDelete(bool flag) { _flagDelete = flag; }
    bool   getFlagDelete() { return _flagDelete; }
    // �������̽�
    virtual void changeState(int state) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void debug() = 0;
    virtual void proc() = 0;
protected:
    //�� ���
    double _x, _y, _z;
    bool   _flagPlayAble = false;         // �÷��̾�� �÷���
    bool   _flagDelete = false;         // �� ���� true�� ������Ʈ �Ŵ������� �����ϰ� �ش� ��ü�� �����ϰ� �ȴ�.
}Object;