#pragma once

// The ordering of these is important
typedef enum TestClientStage {
    TCS_loginWrapper,
    TCS_dbConnect_1,
    TCS_dbConnect_2,
    TCS_dbConnect_3,
    TCS_dbChoosePlayer_1,
    TCS_dbChoosePlayer_2,
    TCS_dbCheckCanStartStaticMap_1,
    TCS_dbCheckCanStartStaticMap_2,
    TCS_dbDisconnect,
    TCS_tryConnect,
    TCS_commReqScene_1,
    TCS_commReqScene_2,
    TCS_commReqScene_3,
    TCS_commReqScene_4,
    TCS_commReqScene_5,
    TCS_allDone,
    TCS_doMapXfer_01,
    TCS_doMapXfer_02,
    TCS_doMapXfer_03,
    TCS_doMapXfer_04,
    TCS_doMapXfer_05,
    TCS_doMapXfer_06,
    MAX_TCS
} TestClientStage;


#ifdef TEST_CLIENT
extern int testClientFakeAuth;
void testRandomDisconnect(TestClientStage stage, char *stagename, char *file, int line);
#    define testClientRandomDisconnect(a) \
    { \
        testRandomDisconnect(a, #a, __FILE__, __LINE__); \
    }
#else
#    define testClientRandomDisconnect(a)
#endif



