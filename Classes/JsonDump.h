//
//  JsonDump.h
//  JsonDump
//
//  Created by Ha Anh Man on 3/8/16.
//
//

#ifndef __JsonDump__JsonDump__
#define __JsonDump__JsonDump__

#include <json/document.h>
#include <json/rapidjson.h>

USING_NS_CC;

class JsonDump {
public:
    static JsonDump* getInstance();
    ValueMap dumpValueMap(rapidjson::Value &data);
    ValueVector dumpValueVector(rapidjson::Value &data);
    ValueVector getArrayNumber(rapidjson::Value &data);
    ValueVector processObjectVV(rapidjson::Value &data);
    ValueMap processObjectVM(rapidjson::Value &data);
    std::string joinVector(std::vector<int> vv, std::string delimiter);
    bool checkKeyExits(ValueMap map, std::string key);
private:
    static JsonDump* instance;
};

#endif /* defined(__JsonDump__JsonDump__) */
