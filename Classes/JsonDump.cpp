//
//  JsonDump.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/8/16.
//
//

#include "JsonDump.h"

JsonDump* JsonDump::instance;
JsonDump* JsonDump::getInstance()
{
    if (instance == nullptr) {
        instance = new JsonDump();
    }
    return instance;
}

/*
 bool IsNull()   const { return flags_ == kNullFlag; }
 bool IsInt()    const { return (flags_ & kIntFlag) != 0; }
 bool IsDouble() const { return (flags_ & kDoubleFlag) != 0; }
 bool IsString() const { return (flags_ & kStringFlag) != 0; }
  bool IsArray()  const { return flags_ == kArrayFlag; }
 bool IsObject() const { return flags_ == kObjectFlag; }

 */
ValueMap JsonDump::dumpValueMap(rapidjson::Value &data) {
    ValueMap vl;
    
    rapidjson::Value::MemberIterator item;
    for (item = data.MemberBegin(); item != data.MemberEnd(); item++)
    {
        std::string key = item->name.GetString();
        if(key == "static_media") {
            CCLOG("aaa");
        }
        if(key.length() == 0) {
            CCLOG("Khong co gia tri key");
        }
        if(item->value.IsNull()) {
            vl[key] = NULL;
            continue;
        }
        if(item->value.IsInt()) {
            vl[key] = item->value.GetInt();
            continue;
        }        
        
        if(item->value.IsDouble()) {
            vl[key] = item->value.GetDouble();
            continue;
        }
        
        if(item->value.IsString()) {
            int so = atoi(item->value.GetString());
            if(so > 0) {
                vl[key] = so;
            } else {
                vl[key] = item->value.GetString();
            }
            continue;
        }
        if(item->value.IsArray()) {
            CCLOG("IsArray: %s", key.c_str());
            if(key == "highlight") {
                vl[key] = getArrayNumber(item->value);
                continue;
            }
//            CCLOG("du lieu la Array: %s", key.c_str());
            ValueVector vvArr = dumpValueVector(item->value);
            vl[key] = vvArr;
            continue;
        }
        
        if(item->value.IsObject()) {
            CCLOG("Object: %s", key.c_str());
            rapidjson::Value &obj = item->value;
            
            //kiem tra xem object nay co key:value hay khong
            bool haskey = false;
            rapidjson::Value::MemberIterator sub;
            for (sub = obj.MemberBegin(); sub != obj.MemberEnd(); sub++) {
                std::string key = sub->name.GetString();
                haskey = key.length() > 0;
                break;
            }
            
            if(haskey) {
                vl[key] = processObjectVM(obj);
            } else {
                vl[key] = processObjectVV(obj);
            }
            
            continue;
        }
    }
    return vl;
}

ValueVector JsonDump::dumpValueVector(rapidjson::Value &data) {
    ValueVector vv;
    for (rapidjson::SizeType i = 0; i < data.Size(); i++) {
        if(data[i].IsObject()) {
            CCLOG("dumpValueVector => IsObject");
        }
        if(data[i].IsArray()) {
            CCLOG("dumpValueVector => IsArray");
        }
        if(data[i].IsInt()) {
            CCLOG("dumpValueVector => IsInt");
            vv.push_back(Value(data[i].GetInt()));
            continue;
        }
        if(data[i].IsDouble()) {
            CCLOG("dumpValueVector => IsDouble");
            vv.push_back(Value(data[i].GetDouble()));
            continue;
        }
        if(data[i].IsString()) {
            CCLOG("dumpValueVector => IsString");
            int so = atoi(data[i].GetString());
            if(so > 0) {
                vv.push_back(Value(so));
            } else {
                vv.push_back(Value(data[i].GetString()));
            }
            continue;
        }
        ValueMap vl = dumpValueMap(data[i]);
        vv.push_back(Value(vl));
    }
    return vv;
}

ValueVector JsonDump::getArrayNumber(rapidjson::Value &data) {
    ValueVector arr;
    for (int i = 0; i < data.Size(); i++) {
        if(data[i].IsInt()) {
            arr.push_back(Value(data[i].GetInt()));
        }
    }
    return arr;
}



ValueVector JsonDump::processObjectVV(rapidjson::Value &data) {
    ValueVector arr;
    rapidjson::Value::MemberIterator sub;
    for (sub = data.MemberBegin(); sub != data.MemberEnd(); sub++) {
        if(sub->value.IsInt()) {
            arr.push_back(Value(sub->value.GetInt()));
        }
    }
    return arr;
}

ValueMap JsonDump::processObjectVM(rapidjson::Value &data) {
    ValueMap map;
    rapidjson::Value::MemberIterator sub;
    for (sub = data.MemberBegin(); sub != data.MemberEnd(); sub++) {
        std::string key = sub->name.GetString();
        if(sub->value.IsObject()) {
            map[key] = dumpValueMap(sub->value);
            continue;
        }
        if(sub->value.IsArray()) {
            map[key] = dumpValueVector(sub->value);
            continue;
        }
        if(sub->value.IsInt()) {
            map[key] = sub->value.GetInt();
            continue;
        }
        
        if(sub->value.IsDouble()) {
            map[key] = sub->value.GetDouble();
            continue;
        }
        
        if(sub->value.IsString()) {
            int so = atoi(sub->value.GetString());
            if(so > 0) {
                map[key] = so;
            } else {
                map[key] = sub->value.GetString();
            }
            continue;
        }
    }
    return map;
}

std::string JsonDump::joinVector(std::vector<int> vv,std::string delimiter) {
    std::stringstream s;
    copy(vv.begin(),vv.end(), std::ostream_iterator<int>(s,delimiter.c_str()));
    std::string str = s.str();
    str = str.substr (0,str.length()-1);    
    return str;
}


bool JsonDump::checkKeyExits(ValueMap map, std::string key) {
    for (auto p : map) {
        if(p.first == key) {
            return true;
        }
    }
    return false;
}
