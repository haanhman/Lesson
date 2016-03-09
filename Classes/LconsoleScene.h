#ifndef __LCONSOLE_SCENE_H__
#define __LCONSOLE_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;
class Lconsole : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void printValue();
    // implement the "static create()" method manually
    CREATE_FUNC(Lconsole);
};

#endif // __LCONSOLE_SCENE_H__
