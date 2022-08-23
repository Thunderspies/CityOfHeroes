
#include "entaiprivate.h"
#include "entity/entity.h"

void aiSetMessageHandler(Entity* e, AIMessageHandler handler){
    if(e && ENTAI(e)){    
        ENTAI(e)->msgHandler = handler;
    }
}

void* aiSendMessagePtr(Entity* e, AIMessage msg, void* data){
    AIMessageHandler handler = ENTAI(e) ? ENTAI(e)->msgHandler : NULL;
    AIMessageParams params;
    
    params.ptrData = data;
    
    if(handler){
        return handler(e, msg, params);
    }
    
    return 0;
}

void* aiSendMessageInt(Entity* e, AIMessage msg, int data){
    AIMessageHandler handler = ENTAI(e) ? ENTAI(e)->msgHandler : NULL;
    AIMessageParams params;
    
    params.intData = data;
    
    if(handler){
        return handler(e, msg, params);
    }

    return 0;
}

void* aiSendMessage(Entity* e, AIMessage msg){
    return aiSendMessageInt(e, msg, 0);
}

                  
