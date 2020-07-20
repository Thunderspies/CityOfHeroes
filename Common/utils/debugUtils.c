#include <utilitieslib/utils/RegistryReader.h>
#include <utilitieslib/assert/assert.h>
#include <stdio.h>
#include <utilitieslib/version/AppRegCache.h>

RegReader reader = 0;

static void duRegReaderLazyInit(){
    if(!reader)
        reader = createRegReader();
    else
        rrClose(reader);
}

int duGetIntDebugSetting(char* keyName){
    int value = 0;
    char buf[1000];

    // Got a valid key name from caller?
    assert(keyName);

    duRegReaderLazyInit();

    // Look in the registry for the give key.
    sprintf(buf,"HKEY_LOCAL_MACHINE\\SOFTWARE\\Cryptic\\%s\\debug", regGetAppName());
    initRegReader(reader, buf);

    // If the value is present, return the value.  Return 0 otherwise.
    if(rrReadInt(reader, keyName, &value)){
        return value;
    }else
        return 0;
}