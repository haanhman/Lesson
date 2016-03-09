//
//  DataSQL.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#include "DataSQL.h"
DataSQL* DataSQL::instance;

DataSQL* DataSQL::getInstance()
{
    if (instance == nullptr) {
        instance = new DataSQL();
        instance->db_path = FileUtils::getInstance()->getWritablePath() + "cards.sqlite";
        instance->openConnect();
    }
    return instance;
}


void DataSQL::openConnect() {
    if(sqlite3_open(this->db_path.c_str(), &this->db) != SQLITE_OK) {
        sqlite3_close(this->db);
        CCLOG("Connect DB co loi");
    } else {
        CCLOG("Connect DB thanh cong");
    }
}

void DataSQL::showError(int result) {
    CCLOG("Database error: %d", result);
    
}

void DataSQL::writeLog(ValueMap data) {
    std::string savePath = FileUtils::getInstance()->getWritablePath() + "abc.plist";
    CCLOG("Log Path: %s", savePath.c_str());
    FileUtils::getInstance()->writeValueMapToFile(data, savePath);
}

ValueMap DataSQL::getLevelInfo(int level_id) {
    ValueMap vl;
    this->stmp = nullptr;
    std::string query = "SELECT id,jsondata,name,lang_id,level_type,sample_level FROM LevelInfo WHERE id = ?";
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if(result != SQLITE_OK) {
        this->showError(result);
        return vl;
    }
    
    sqlite3_bind_int(this->stmp, 1, level_id);
    if (sqlite3_step(this->stmp) == SQLITE_ROW) {
        vl["id"] = sqlite3_column_int(this->stmp, 0);
        vl["jsondata"] = __String::create((const char*)sqlite3_column_text(this->stmp, 1))->getCString();
        vl["name"] = __String::create((const char*)sqlite3_column_text(this->stmp, 2))->getCString();
        vl["lang_id"] = sqlite3_column_int(this->stmp, 3);
        vl["level_type"] = sqlite3_column_int(this->stmp, 4);
        vl["sample_level"] = sqlite3_column_int(this->stmp, 5);
    }
    return vl;
}

ValueVector DataSQL::getCards(int level_id, std::vector<int> listCardId) {
    ValueVector data;
    this->stmp = nullptr;
    
//    std::string str_cards = "2817,2370,2366,2371,2870,11878,2865,11874";
    std::string str_cards = JsonDump::getInstance()->joinVector(listCardId, ",");
    std::string query = __String::createWithFormat("SELECT * FROM listCards WHERE level_id = ? AND id IN (%s)", str_cards.c_str())->getCString();
    int result = sqlite3_prepare_v2(this->db, query.c_str(), -1, &this->stmp, nullptr);
    if(result != SQLITE_OK) {
        this->showError(result);
        return data;
    }
    
    sqlite3_bind_int(this->stmp, 1, level_id);
    while (sqlite3_step(this->stmp) == SQLITE_ROW) {
        ValueMap map;
        map["id"] = sqlite3_column_int(this->stmp, 0);
        map["card_detail"] = __String::create((const char*)sqlite3_column_text(this->stmp, 1))->getCString();
        map["name"] = __String::create((const char*)sqlite3_column_text(this->stmp, 2))->getCString();
        data.push_back(Value(map));
    }
    return data;
}
