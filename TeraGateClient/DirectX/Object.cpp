#include "Object.h"
Object::Object()
{
    _pos.x = 0;
    _pos.y = 0;
    _pos.z = 0;
    _rot.x = 0;
    _rot.y = 0;
    _rot.z = 0;
}
Object::~Object()
{
}
void Object::setPos(float x, float y, float z) { _pos = {x, y, z}; };
void Object::setPosX(float x) { _pos.x = x; }
void Object::setPosY(float y) { _pos.y = y; }
void Object::setPosZ(float z) { _pos.z = z; }
void Object::setRot(float x, float y, float z) { _rot = {x, y, z}; }
void Object::setRotX(float x){_rot.x = x;}
void Object::setRotY(float y){_rot.y = y;}
void Object::setRotZ(float z){_rot.z = z;}
void Object::move(float x, float y, float z) { _pos.x += x; _pos.y += y; _pos.z += z; };
void Object::moveX(float x)   { _pos.x += x; }
void Object::moveY(float y)   { _pos.y += y; }
void Object::moveZ(float z)   { _pos.z += z; }
D3DXVECTOR3 Object::getPos()  { return _pos; }
float Object::getPosX() { return _pos.x; }
float Object::getPosY() { return _pos.y; }
float Object::getPosZ() { return _pos.z; }
D3DXVECTOR3 Object::getRot()  { return _rot; }
float Object::getRotX(){ return _rot.x; }
float Object::getRotY(){ return _rot.y; }
float Object::getRotZ(){ return _rot.z; }
D3DXMATRIX Object::getMtxWorld()
{
    D3DXMATRIX* mtx;

    // 맥스 때문에 회전
    D3DXMATRIX mtxExport;
    D3DXMatrixRotationX(&mtxExport, DEGREE_TO_RADIAN(-90));

    // 회전
    D3DXMATRIX mtxRotation[3];
    D3DXMatrixRotationX(&mtxRotation[0], DEGREE_TO_RADIAN(_rot.x));
    D3DXMatrixRotationY(&mtxRotation[1], DEGREE_TO_RADIAN(_rot.y));
    D3DXMatrixRotationZ(&mtxRotation[2], DEGREE_TO_RADIAN(_rot.z));

    // 오브젝트 월드좌표만큼 이동
    D3DXMATRIX mtxPosition;
    D3DXMatrixTranslation(&mtxPosition, _pos.x, _pos.y, _pos.z);

    // 스케일링
    D3DXMATRIX mtxScaling;
    D3DXMatrixScaling(&mtxScaling, 1, 1, 1);

    return mtxExport * mtxRotation[0] * mtxRotation[1] * mtxRotation[2] * mtxPosition * mtxScaling;
}
void Object::update()
{
}
void Object::setArkDes(D3DXVECTOR3 ark, D3DXVECTOR3 des)
{
    
}