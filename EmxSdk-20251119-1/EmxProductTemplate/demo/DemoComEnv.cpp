#include "EmxModules.hpp"

using namespace Emx;

int main() {
    OemId oemId;
    oemId.Load();
    emxlogi("oemId: %s\n", oemId.Get());

    EpSn epSn;
    epSn.Load();
    emxlogi("epSn: %s\n", epSn.Get());
    
    Language language;
    language.Load();
    emxlogi("language: %d\n", language.Get());
    return 0;
}