//
//  LessonData.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/7/16.
//
//

#include "LessonData.h"
#include "DataSQL.h"
#include "JsonDump.h"

#include <json/document.h>
#include <json/rapidjson.h>

LessonData* LessonData::instance;

LessonData* LessonData::getInstance()
{
    if (instance == nullptr) {
        instance = new LessonData();
    }
    return instance;
}



/*
 slide1: Lesson 1
 8 slide tiep theo (anh - text)
 */
void LessonData::getLessonFromLevel(int level_id, int lesson_index) {
    this->level_id = level_id;
    ValueMap vl = DataSQL::getInstance()->getLevelInfo(this->level_id);
    
    std::string json_data = vl["jsondata"].asString();
    rapidjson::Document document;
    rapidjson::Document phonic_highlight;
    document.Parse<0>(json_data.c_str());
    if(document.HasParseError()) {
        CCLOG("Parse json error");
        return;
    }
    
    if(document.HasMember("listlesson")) {
        rapidjson::Value &listlesson = document["listlesson"];
        rapidjson::Value &selected_lesson = listlesson[lesson_index];
        
        
        this->lesson_name = selected_lesson["name"].GetString();
        this->logData["lesson_name"] = this->lesson_name;
        
        //kiem tra co phonic_cate hay khong
        rapidjson::Value &phonic_cate = selected_lesson["phonic_cate"];
        if(!phonic_cate.IsNull()) {
            this->vm_phonic_cate = JsonDump::getInstance()->dumpValueMap(phonic_cate);
            this->vm_phonic_cate["short_title"] = __String::createWithFormat("Short \"%s\"", this->vm_phonic_cate.at("name").asString().c_str())->getCString();
            this->logData["vm_phonic_cate"] = this->vm_phonic_cate;
        }
        
        
        //danh sach phonic neu co
        rapidjson::Value &phonic = selected_lesson["phonic"];
        if(!phonic.IsNull()) {
            this->listPhonic = JsonDump::getInstance()->dumpValueVector(phonic);
            for (int i = 0; i < this->listPhonic.size(); i++) {
                ValueMap vl = this->listPhonic[i].asValueMap();
                phonic_highlight.Parse<0>(vl.at("highlight").asString().c_str());
                ValueVector arr_highlight;
                if(phonic_highlight.Size()) {
                    for (int j = 0; j < phonic_highlight.Size(); j++) {
                        arr_highlight.push_back(Value(phonic_highlight[j].GetInt()));
                    }
                }
                vl["highlight"] = arr_highlight;
                this->listPhonic[i] = vl;
            }
            this->logData["listPhonic"] = this->listPhonic;
        }
        
        
        ValueVector listGroup;
        std::vector<int>slowCard;
        std::vector<int>fastCard;
        
        //lay thong tin group
        rapidjson::Value &groups = selected_lesson["groups"];
        if(!groups.IsNull() && groups.IsArray()) {
            listGroup = JsonDump::getInstance()->dumpValueVector(groups);
            
            //lay thong tin card slow, fast
            for (auto g : listGroup) {
                ValueMap item = g.asValueMap();
                int is_phonic = item.at("is_phonic").asInt();
                int sightword = item.at("sightword").asInt();
                ValueVector rows = item.at("cards").asValueVector();
                for (auto row : rows) {
                    ValueMap card = row.asValueMap();
                    int card_id = card.at("card_id").asInt();
                    if(is_phonic == 0 && sightword == 0) {
                        slowCard.push_back(card_id);
                    }
                    fastCard.push_back(card_id);
                }
            }
            
            this->logData["listGroup"] = listGroup;
        }
        getSlowCard(slowCard);
//        DataSQL::getInstance()->writeLog(this->logData);
    }
}
//SELECT * FROM listCards WHERE level_id = 89 AND id IN (2817,2370,2366,2371,2870,11878,2865,11874)

ValueMap LessonData::getSlowCard(std::vector<int> slowCard) {
    
    std::string level_key = "easy";
    
    this->logData.clear();
    ValueMap map;
    ValueMap audio_sound;//key = card_id
    ValueMap card_audio;//key = text_id
    ValueMap fast_audio;//key = text_id
    ValueMap card_image;//key = card_id
    ValueMap card_image_video;//key = card_id
    ValueVector result = DataSQL::getInstance()->getCards(this->level_id, slowCard);
    for(auto a : result) {
        ValueMap item = a.asValueMap();
        int card_id = item.at("id").asInt();
        std::string card_name = item.at("name").asString();
        std::string card_detail = item.at("card_detail").asString();
        rapidjson::Document document;
        document.Parse<0>(card_detail.c_str());
        rapidjson::Value &json = document;
        ValueMap map = JsonDump::getInstance()->dumpValueMap(json);
        map["name"] = card_name;
        std::string id_key = __String::createWithFormat("%i", card_id)->getCString();
        
        
        
//MARK: lay thong tin audio theo tung text_id
        ValueVector audio = map.at("audio").asValueVector();
        for (int i = 0; i<audio.size(); i++) {
            ValueMap vl = audio[i].asValueMap();
            if(vl.at("type").asInt() == 2) {
                ValueVector aSound;
                if ( JsonDump::getInstance()->checkKeyExits(audio_sound, id_key) ) {
                    aSound = audio_sound.at(id_key).asValueVector();
                }
                aSound.push_back(Value(vl));
                audio_sound[id_key] = aSound;
            } else {
                int id_text = vl.at("id_text").asInt();
                std::string text_key = __String::createWithFormat("%i", id_text)->getCString();
                
                float fastAudio = false;
                if(JsonDump::getInstance()->checkKeyExits(vl, "fast")) {
                    fastAudio = vl.at("fast").asInt() == 1;
                }
                
                if(fastAudio) {
                    ValueVector faudio;
                    if ( JsonDump::getInstance()->checkKeyExits(fast_audio, text_key) ) {
                        faudio = fast_audio.at(text_key).asValueVector();
                    }
                    faudio.push_back(Value(vl));
                    fast_audio[text_key] = faudio;
                } else {
                    
                    ValueVector text_audio;
                    if ( JsonDump::getInstance()->checkKeyExits(card_audio, text_key) ) {
                        text_audio = card_audio.at(text_key).asValueVector();
                    }
                    text_audio.push_back(Value(vl));
                    card_audio[text_key] = text_audio;
                }
            }
        }
        
//MARK: lay thong tin anh
        ValueVector image = map.at("image").asValueMap().at("normal").asValueVector();
        card_image[id_key] = image;
        
//MARK: lay thong tin anh
        ValueVector image_video = map.at("image_video").asValueMap().at(level_key).asValueVector();
        card_image_video[id_key] = image_video;
        
    }
    
    this->logData["audio_sound"] = audio_sound;
    this->logData["card_audio"] = card_audio;
    this->logData["fast_audio"] = fast_audio;
    this->logData["card_image"] = card_image;
    this->logData["card_image_video"] = card_image_video;
    
    int card_in_group = 4;
    for (int i; i < slowCard.size(); i++) {
        int card_id = slowCard[i];
    }
    
    DataSQL::getInstance()->writeLog(this->logData);
    return map;
}


