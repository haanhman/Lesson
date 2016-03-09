#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"
#include <json/document.h>
#include <json/rapidjson.h>
#include "sqlite3.h"
#include "LessonData.h"

USING_NS_CC;

Label *label;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size sceneSize = Director::getInstance()->getVisibleSize();

    
    auto background = LayerColor::create(Color4B::BLACK);
    background->setContentSize(sceneSize);
//    background->setAnchorPoint(Vec2(0.5f, 0.5f));
//    background->setPosition(Vec2(sceneSize.width/2, sceneSize.height/2));
    addChild(background);
    
    label = Label::createWithTTF("Ha Anh Man", "fonts/Marker Felt.ttf", 24);
    label->setColor(Color3B::RED);
    label->setPosition(Vec2(sceneSize.width/2, sceneSize.height/2));
    addChild(label);
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegin, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    
    auto btn = ui::Button::create("Button_Normal.png", "Button_Press.png", "Button_Disable.png");
    btn->setAnchorPoint(Vec2(0.5f, 0.5f));
    btn->setPosition(Vec2(sceneSize.width/2, sceneSize.height*0.3f));
    
    btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                label->setString("Click ok");
//                readPlistFile();
//                parseJSON();
                loadData();
                break;
            default:
                break;
        }
    });

    
    addChild(btn);
    
    
    //text file
    auto textFile = ui::TextField::create("Please enter your name", "fonts/Marker Felt.ttf", 20);
    textFile->setAnchorPoint(Vec2(0.5f, 0.5f));
    textFile->setPosition(Vec2(sceneSize.width/2, sceneSize.height*0.7f));
    textFile->setPasswordEnabled(true);
    addChild(textFile);
    
    
    
    
    return true;
}

void HelloWorld::loadData() {
    
    LessonData::getInstance()->getLessonFromLevel(89, 1);
    return;
    
    std::string jsondata = "";
    sqlite3 *db = nullptr;
    std::string db_path = FileUtils::getInstance()->getWritablePath() + "cards.sqlite";
    
    CCLOG("DB path: %s", db_path.c_str());
    
    if(sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
        CCLOG("Error");
        sqlite3_close(db);
    } else {
        CCLOG("Connect DB thanh cong");
    }
    sqlite3_stmt *stmp = nullptr;
    std::string query = "SELECT * FROM LevelInfo WHERE id = 85";
    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmp, nullptr);
    if(result == SQLITE_OK) {
        
        if (sqlite3_step(stmp) == SQLITE_ROW) {
            int col_id  = sqlite3_column_int(stmp, 0);
            jsondata = __String::create((const char*)sqlite3_column_text(stmp, 1))->getCString();
            std::string name  = __String::create((const char*)sqlite3_column_text(stmp, 3))->getCString();
            CCLOG("ID: %i", col_id);
            CCLOG("Name: %s", name.c_str());
            parseJSON(jsondata);
        }
    } else {
        CCLOG("Error: %d", result);
    }
    
    
    
//    result=sqlite3_prepare_v2(db,query.c_str(), -1, &stmp, nullptr);
//    if (result == SQLITE_OK) {
//        if (sqlite3_step(ppStmt1) == SQLITE_ROW) {
//            image_path = __String::create((const char*)sqlite3_column_text(ppStmt1, 2))->getCString();
//            audio_normal = __String::create((const char*)sqlite3_column_text(ppStmt1, 3))->getCString();
//            audio_sound = __String::create((const char*)sqlite3_column_text(ppStmt1, 4))->getCString();
//        }
//    }
    
}

void HelloWorld::readPlistFile() {
    cocos2d::FileUtils *fileManager = FileUtils::getInstance();
    std::string plist_file = fileManager->fullPathForFilename("policy_vn.plist");
    CCLOG("plist file: %s", plist_file.c_str());
    
    std::string document_path = fileManager->getWritablePath();
    CCLOG("document path: %s", document_path.c_str());
    ValueMap vm = fileManager->getValueMapFromFile(plist_file);
    ValueVector vv = vm.at("list_data").asValueVector();
    int count = vv.size();
    if(count > 0) {
        for(int i = 0; i<count;i++) {
            ValueMap item = vv.at(i).asValueMap();
            std::string q = item.at("q").asString();
            std::string a = item.at("a").asString();
            CCLOG("Q: %s", q.c_str());
            CCLOG("A: %s\\n", a.c_str());
        }
    }
    
    CCLOG("Tong so phan tu: %i", count);
}



void HelloWorld::parseJSON(std::string json) {
    rapidjson::Document document;
    document.Parse<0>(json.c_str());
    if(document.HasParseError()) {
        CCLOG("Parse json error");
    } else {
        if(document.IsObject()) {
            if(document.HasMember("static")) {
                rapidjson::Value &vm_static = document["static"];
                /*
                 "voices" : 3,
                 "total_words" : 36,
                 "video_mouth" : 0,
                 "animation" : 0,
                 "audio" : 723,
                 "total_card" : 0,
                 "total_sentence" : 0,
                 "total_lesson" : 0,
                 "image" : 199,
                 "video" : 80
                 */
                CCLOG("voices: %d", vm_static["voices"].GetInt());
                CCLOG("total_words: %d", vm_static["total_words"].GetInt());
                CCLOG("video_mouth: %d", vm_static["video_mouth"].GetInt());
                CCLOG("animation: %d", vm_static["animation"].GetInt());
                CCLOG("total_card: %d", vm_static["total_card"].GetInt());
                CCLOG("total_sentence: %d", vm_static["total_sentence"].GetInt());
                CCLOG("total_lesson: %d", vm_static["total_lesson"].GetInt());
                CCLOG("image: %d", vm_static["image"].GetInt());
                CCLOG("video: %d", vm_static["video"].GetInt());
            }
            CCLOG("\n============ LESSONS ============\n");
            if(document.HasMember("listlesson")) {
                
                std::vector<cocos2d::ValueMap> lessons;

                rapidjson::Value &listlesson = document["listlesson"];
                int total_lesson = listlesson.Size();
                CCLOG("Tong so lesson: %i", total_lesson);
                if(total_lesson > 0) {
                    for(int i = 0; i<total_lesson;i++) {
                        if(i > 3) {
                            break;
                        }
                        std::string lesson_id = listlesson[i]["id"].GetString();
                        std::string lesson_name = listlesson[i]["name"].GetString();
                        
                        ValueMap vl;
                        vl["id"] = lesson_id;
                        vl["name"] = lesson_id;
                        
                        lessons.push_back(vl);
                        
                        CCLOG("Lesson ID: %s - Name: %s", lesson_id.c_str(), lesson_name.c_str());
                    }
                }
//                CCLOG("tong: %lu", lessons.size());
//                std::string str(lessons.begin(), lessons.end());
//                __Array *arr = __Array::create();
                
                
//                CCDictionary *m_game_data = new CCDictionary();
//                __Array::createWithObject(lessons);
                
                
            }
            
            
            
//            listlesson
            
        }
        
        
        
    }
    
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event) {
    Vec2 location = touch->getLocation();
    __String *newLocation = __String::createWithFormat("onTouchMoved X: %f, Y: %f", location.x, location.y);
    label->setString(newLocation->getCString());
}
void HelloWorld::onTouchEnded(Touch *touch, Event *event) {
    Vec2 location = touch->getLocation();
    __String *newLocation = __String::createWithFormat("onTouchEnded X: %f, Y: %f", location.x, location.y);
    label->setString(newLocation->getCString());
}
bool HelloWorld::onTouchBegin(Touch *touch, Event *event) {
    Vec2 location = touch->getLocation();
    __String *newLocation = __String::createWithFormat("onTouchBegin X: %f, Y: %f", location.x, location.y);
    label->setString(newLocation->getCString());
    return true;
}