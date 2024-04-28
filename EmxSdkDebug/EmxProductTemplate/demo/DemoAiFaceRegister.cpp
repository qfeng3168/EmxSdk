//
// Created by tangfahong on 2024/4/22.
//
#include "EmxMedia.hpp"

using namespace Emx;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Please enter the parameters:\n");
        printf("register:./DemoAiFaceRegister 0 id imgPath\n");
        printf("unregister:./DemoAiFaceRegister 1 id\n");
        printf("sync library:./DemoAiFaceRegister 2 libJsonPath\n");
        return -1;
    }
    MediaAi demo(0);

    if(!strcmp(argv[1],"0")){
        //register
        Json::Value faceRegister;
        Json::Value array;
        Json::Value out;
        array.resize(0);
        Json::Value info;
        info["id"] = atoi(argv[2]);
        if(argc < 4){
            info["isImg"] = false;
            info["path"] = "";
        }else{
            info["isImg"] = true;
            info["path"] = argv[3];
        }
        array.append(info);
        faceRegister["faceRegister"] = array;
        ErrCodeE e = demo.Ctrl(faceRegister,out);
        if(e == ErrCodeE::Success){
            std::string outString;
            EasyJson::Serialize(out, outString);
            printf("outString:%s \n",outString.c_str());

        }
        printf("register ret=%d \n",(int)e);

    }else if(!strcmp(argv[1],"1")){
        //unregister
        Json::Value faceUnregister;
        Json::Value array(Json::arrayValue);
        Json::Value out;
        Json::Value info;
        for (int i = 2; i < argc; i++) {
            array.append(atoi(argv[i]));
        }
        faceUnregister["faceUnregister"] = array;
        ErrCodeE e = demo.Ctrl(faceUnregister,out);
        printf("unregister ret=%d \n",(int)e);
    }else if(!strcmp(argv[1],"2")){
        //sync library
        Json::Value library;
        Json::Value out;
        if (EasyJson::Load(argv[2], library) != ErrCodeE::Success) {
            printf("load %s failed\n", argv[2]);
            return -1;
        }

        ErrCodeE e = demo.Ctrl(library,out);
        printf("sync library ret=%d \n",(int)e);
    }

    return 0;
}