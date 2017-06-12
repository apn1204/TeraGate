#include "ObjectMinion.h"
ObjectMinion::ObjectMinion()
{
}
ObjectMinion::~ObjectMinion()
{
}
void ObjectMinion::update()
{
    static int aiUpdateTerm = 0;
    aiUpdateTerm += RANDOM_M(1, 3);
    if (150 < aiUpdateTerm)
    {
        _rot.y = RANDOM_M(0, 360);
        aiUpdateTerm = 0;
    }
    static float movement     = 1.0f;
    static float rotationUnit = 0.2f;

    if (pow(1000, 2) < pow(_pos.x + movement * sin(DEGREE_TO_RADIAN(_rot.y)), 2) + pow(_pos.z + movement * cos(DEGREE_TO_RADIAN(_rot.y)), 2))
    {
        _rot.y += RANDOM_M(0, 360);
    }
    else
    {
        _pos.x += movement * sin(DEGREE_TO_RADIAN(_rot.y));
        _pos.z += movement * cos(DEGREE_TO_RADIAN(_rot.y));
    }
}