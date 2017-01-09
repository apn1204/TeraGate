#pragma once
#include"Object.h"
//�� ObjectManager Ŭ���� ���
//  ������Ʈ�� ����, ����, ����, ��ȣ�ۿ� �� �����ϴ� Ŭ����.
//
//�� ������Ʈ�� �����ϴ� ���
//  1. �ΰ��� ���� �ʱ�ȭ�� ������Ʈ�� �����Ѵ�.
//  2. ������Ʈ�� �ٸ� ������Ʈ�� �����Ѵ�.
//
//�� ObjectManager�� �̱����̾�� �ϴ� ����
//  �ΰ��� ���Ӹ� �ƴ϶� ������Ʈ�κ����� ������ �����ϰ��ϱ� ���ؼ�.
class ObjectManager
{
    //�� �̱���
private:
    ObjectManager();
public:
    static ObjectManager* mInstance;
    static ObjectManager* getInstance();
    //�� �޼ҵ�
public:
    ~ObjectManager();
    void add(Object* object);
    void deleteObject();
    void setAllDeleteFlagTrue();
    void proc(HWND, UINT, WPARAM, LPARAM);
    void update();
    void render();
    void debug();
    void order();
    void updateDamage();
    int  getQuantity();

    //�� ���
    const static int MAX_OBJECT_QUANTITY = 1000;// ������Ʈ �ִ� ���� ����
protected:
    //�� ����
    Object* _object[MAX_OBJECT_QUANTITY];// Object�������� �迭. STL�� ���� �������� ���� ������ �迭 ��ü�� �����Ҵ����� ����.
    int     _objectQuantity = 0;         // ������ ������Ʈ ����.    
};