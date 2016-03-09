//
//  DataSQL.h
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#ifndef __DataSQL_H__
#define __DataSQL_H__
#include "cocos2d.h"
#include "sqlite3.h"
#include "JsonDump.h"

USING_NS_CC;

class DataSQL {
public:
    
    
    
    static DataSQL* getInstance();
    //LevelInfo
    ValueMap getLevelInfo(int level_id);
    ValueVector getCards(int level_id, std::vector<int> listCardId);
    
    
    
    void writeLog(ValueMap data);
    
private:
    static DataSQL* instance;
    sqlite3 *db;
    sqlite3_stmt *stmp;
    std::string db_path;
    void openConnect();
    void showError(int result);
};


#endif /* __DataSQL_H__ */
