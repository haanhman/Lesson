#include "LconsoleScene.h"



//#include <platform/apple/CCFileUtils-apple.h>
USING_NS_CC;
Scene* Lconsole::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Lconsole::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Lconsole::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    printValue();
    return true;
}

void Lconsole::printValue() {
    
    
//    ValueMap m=FileUtils::getInstance()->getValueMapFromFile("duongdan/abc.plist");
//        for (auto it = m.begin(); it != m.end(); ++it) {
//                const std::string &c_namex = it->first;
//                std::cout<<"\nkey:"<<c_namex;
//            }
    
//    ValueMap m=FileUtils::getInstance()->getValueMapFromFile("duongdan/abc.plist");
//    for (auto it = m.begin(); it != m.end(); ++it) {
//        const std::string &c_namex = it->first;
//        std::cout<<"\nkey:"<<c_namex;
//    }
    
    std::vector<int> listCard;
    listCard.push_back(1);
    listCard.push_back(2);
    listCard.push_back(3);
    listCard.push_back(4);

    
    ValueMap vlStudent;
    vlStudent["name"] = "Mike";
    vlStudent["age"] = 34;
    vlStudent["address"] = "San Francisco";
    
    std::vector<std::string> keys;
    for (auto p : vlStudent) {
        CCLOG("key: %s", p.first.c_str());
    }
    
    
    
//    Map<std::string, Value> hasMap;
    
//    Map<std::string, Sprite*> map0;
    
//    hasMap.insert("hoten", "Ha Anh Man");
//    hasMap.insert("tuoi", Value(27));
//    hasMap.insert("diachi", Value("Ha Noi"));
//    std::vector<std::string> vtKeys = hasMap.keys();
//    for(std::string key : vtKeys) {
//        CCLOG("Key: %s", key.c_str());
//    }
    return;
    /* Set up vector to hold chars a-z */
    ValueVector vData;
    
    for (int i = 1; i <= 10; i++) {
        ValueMap vl;
        std::string key = __String::createWithFormat("image_%i", i)->getCString();
        std::string value = __String::createWithFormat("noi dung key %i", i)->getCString();
        vl[key] = value;
        
        std::string key2 = __String::createWithFormat("audio_%i", i)->getCString();
        std::string value2 = __String::createWithFormat("mp3_%i.mp3", i)->getCString();
        vl[key2] = value2;
        vData.push_back(Value(vl));
    }
    
    
    

    std::string savePath = FileUtils::getInstance()->getWritablePath() + "abc.plist";
//    FileUtilsApple::getInstance()->writeValueVectorToFile(vData, savePath.c_str());
    CCLOG("Save path: %s", savePath.c_str());
    ValueMap vl;
    std::string key = "key";
    std::string value = "value";
    
    ValueMap vl2;
    vl2["abc"] = vData;
    
    vl[key] = vl2;
    FileUtils::getInstance()->writeToFile(vl, savePath.c_str());
    
//    FileUtilsApple::getInstance()->writeToFile(vlabc, savePath.c_str());
    
    
    /*
     
     bool FileUtilsApple::writeToFile(ValueMap& dict, const std::string &fullPath)
     {
     return writeValueMapToFile(dict, fullPath);
     }
     */
    
//    std::for_each(vData.begin(), vData.end(), [] (const char c) {std::cout << c << " ";});
//    std::cout << &vData << std::endl;
    
    
    
//        path.push_back(ch);
    
    /* Print path vector to console */
//    std::copy(vData.begin(), vData.end(), std::ostream_iterator<ValueMap>(std::cout, " "));
}