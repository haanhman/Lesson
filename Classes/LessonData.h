//
//  LessonData.h
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#ifndef __LESSONDATA_H__
#define __LESSONDATA_H__
#include "cocos2d.h"
USING_NS_CC;


class LessonData {
    public:
        // bien nay de luu du lieu hien co => debug
        ValueMap logData;
    
        ValueMap vm_phonic_cate;
        std::string lesson_name;
        ValueVector listPhonic;
    
        static LessonData* getInstance();
        void getLessonFromLevel(int level_id, int lesson_index);
        ValueMap getSlowCard(std::vector<int> slowCard);
    private:
        static LessonData* instance;
        int level_id;
};
#endif /* __LESSONDATA_H__ */
