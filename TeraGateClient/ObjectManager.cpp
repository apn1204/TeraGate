#include"ObjectManager.h"
//�� �̱���
ObjectManager* ObjectManager::mInstance = NULL;
ObjectManager* ObjectManager::getInstance()
{
    if (mInstance == NULL)
        mInstance = new ObjectManager;
    return mInstance;
}
ObjectManager::ObjectManager()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        _object[i] = NULL;
}
//�� �޼ҵ�
ObjectManager::~ObjectManager()
{
    delete[] _object;
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        _object[i] = NULL;
}
// ����� ����
// mObjectManager.add(new ObjectThresh);
void ObjectManager::add(Object* object)
{
    // ������Ʈ �ִ� ������ �̸��̶��
    if (_objectQuantity < MAX_OBJECT_QUANTITY)
    {
        // ������Ʈ�� �����͹迭�� �ְ�
        // ������Ʈ ������ 1 �����Ѵ�.
        _object[_objectQuantity] = object;
        _objectQuantity++;
    }
}
/*
������Ʈ�� �÷���-����Ʈ�� üũ�� �޸𸮸� �����ϰ� NULL���� �ִ´�.
������Ʈ ������ 0���� �ʱ�ȭ �Ѵ�.
�ε����� ������� �ʴٸ� ������Ʈ ������ ī��Ʈ�Ѵ�.
*/
void ObjectManager::deleteObject()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL && _object[i]->getFlagDelete())
        {
            delete _object[i];
            _object[i] = NULL;
        }
    _objectQuantity = 0;
    for (int a = 0; a<MAX_OBJECT_QUANTITY; a++)
        if (_object[a] != NULL)
            _objectQuantity++;
}
// ��� ������Ʈ�� �÷���-����Ʈ�� ������ �����.
// ���� deleteObject()�� ȣ��� �� ��� ������Ʈ�� �����ǰ� �ȴ�.
void ObjectManager::setAllDeleteFlagTrue()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL)
            _object[i]->setFlagDelete(TRUE);
}
void ObjectManager::proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < _objectQuantity; i++)
    {
//        ������Ʈ���� �־����� �̺�Ʈ ó��
//        _object[i]->proc(hWnd, message, wParam, lParam);
    }
}
void ObjectManager::update()
{
    // ObjectManager Ŭ���� ������
    // update() �������� �Լ�ȣ�� ������ �� ��������.
    // 1. ��� ������Ʈ�� ������Ʈ�� ���� �÷��׸� �����Ѵ�.
    // 2. ���� �÷��װ� ���� ������Ʈ�� �����Ѵ�.
    // 3. ��������ȭ�� ���� ������Ʈ �迭�� ���������Ѵ�.
    for (int i = 0; i<MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL)
            _object[i]->update();
    deleteObject();
    order();
}
void ObjectManager::render()
{
    for (int i = 0; i<MAX_OBJECT_QUANTITY; i++)
    {
        if (_object[i] != NULL)
            _object[i]->render();
    }
}
void ObjectManager::debug()
{
}
void ObjectManager::order()
{
    /*
    �����۾� ����� Big(O)���� ����ȭ�� �ʿ�������..
    ��ɻ��� ������ ����.

    ����ִ� �ε��� ���� üũ�� ���� �ε����� ����´�.
    ex) ���� �� �ε���
    mObject[0] = NULL;
    mObject[1] = ����;
    ex) ���� �� �ε���
    mObject[0] = ����;
    mObject[1] = NULL;
    */
    for (int a = 0; a<MAX_OBJECT_QUANTITY; a++)
        for (int b = 0; b < MAX_OBJECT_QUANTITY - 1; b++)
        {
            if (_object[b] == NULL && _object[b + 1] != NULL)
            {
                _object[b] = _object[b + 1];
                _object[b + 1] = NULL;
            }
        }
}
void ObjectManager::updateDamage()
{
}
int  ObjectManager::getQuantity()
{
    return _objectQuantity;
}