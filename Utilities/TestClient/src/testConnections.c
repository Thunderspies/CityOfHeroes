#include "testConnections.h"
#include "mover.h"
#include "clientcomm/clientcomm.h"
#include <stdio.h>
#include "gameData/randomCharCreate.h"
#include "testClientInclude.h"
#include "testClient/TestClient.h"
#include <utilitieslib/utils/utils.h>
#include <utilitieslib/utils/SimpleParser.h>
#include "entity/entVarUpdate.h"

#define commAddInput(s) printf("commAddInput(\"%s\");\n", s); commAddInput(s);

int drop_stage = -1; // At what stage we will disconnect

int ticks_until_move=25; // 5 seconds
static int static_map=-1;


static int smlist[] = {1, 5, 6}; //, 7, 8, 9, 12, 13, 14, 18, 19, 20};

// The static maps in maps.db (2060)
// @todo this is brittle and should be replaced withs something more robust

// list a set of the startup static maps first to make it easier to restrict mapmoves
// to selecting amongst a smaller subset of maps, and those that may already be running
static int smlist_base[] =
{
    1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,29,36,37,38,39,40,42,51,
    55,56,60,61,62,63,64,65,66,67,68,69,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87
};

// all the static maps (and their rollover instances) from maps.db (2060)
// if we want to stress launching lots of static maps and instances of them
static int static_map_list_all[] =
{
    1,101,102,103,104,105,106,107,2,3,301,302,303,304,305,306,307,308,309,310,
    311,312,313,314,4,401,402,403,404,405,406,407,408,409,410,411,412,413,414,5,
    501,502,503,504,505,506,507,6,601,602,603,604,7,701,702,703,704,8,801,802,
    803,804,9,901,902,903,904,10,1001,1002,1003,11,1101,1102,1103,12,1201,1202,1203,1204,
    1205,13,1301,1302,1303,14,1401,1402,1403,15,1501,1502,1503,16,1601,1602,1603,1604,1605,1606,
    1607,1608,1609,1610,1611,1612,1613,1614,1615,17,1701,1702,1703,1704,1705,1706,1707,1708,1709,1710,
    1711,1712,1713,1714,1715,18,1801,1802,1803,19,1901,1902,1903,20,2001,2002,2003,21,2101,2102,
    2103,22,2201,2202,2203,2204,2205,2206,2207,2208,2209,2210,2211,2212,2213,2214,2215,23,2301,2302,
    2303,2304,2305,24,25,26,27,28,29,2901,2902,2903,2904,2905,2906,2907,30,31,32,33,
    34,35,36,3601,3602,3603,3604,3605,3606,3607,3608,3609,3610,3611,3612,3613,3614,3615,3616,3617,
    3618,3619,3620,3621,3622,3623,3624,3625,37,3701,3702,3703,3704,3705,3706,3707,3708,3709,3710,3711,
    3712,3713,3714,3715,3716,3717,3718,3719,3720,3721,3722,3723,3724,3725,38,3801,3802,3803,3804,3805,
    3806,3807,3808,3809,3810,3811,3812,3813,3814,3815,3816,3817,3818,3819,3820,3821,3822,3823,3824,3825,
    39,3901,3902,3903,3904,3905,3906,3907,3908,3909,3910,3911,3912,3913,3914,3915,3916,3917,3918,3919,
    3920,3921,3922,3923,3924,3925,40,4001,4002,4003,4004,4005,4006,4007,4008,4009,4010,4011,4012,4013,
    4014,4015,4016,4017,4018,4019,4020,4021,4022,4023,4024,4025,4026,4027,4028,4029,4030,4031,4032,4033,
    4034,4035,4036,4037,4038,4039,4040,4041,4042,4043,4044,4045,4046,4047,4048,4049,4050,42,4201,4202,
    4203,4204,4205,4206,4207,4208,4209,4210,4211,4212,4213,4214,4215,4216,4217,4218,4219,4220,4221,4222,
    4223,4224,4225,50,51,5101,5102,5103,5104,5105,5106,5107,5108,5109,5110,5111,5112,5113,5114,5115,
    5116,5117,5118,5119,5120,5121,5122,5123,5124,5125,55,5501,5502,5503,5504,5505,5506,5507,56,5601,
    5602,5603,5604,5605,5606,5607,60,6001,6002,6003,6004,6005,6006,6007,61,6101,6102,6103,6104,6105,
    6106,6107,62,6201,6202,6203,6204,6205,6206,6207,63,6301,6302,6303,6304,6305,6306,6307,64,6401,
    6402,6403,6404,6405,6406,6407,65,6501,6502,6503,6504,6505,6506,6507,66,6601,6602,6603,6604,6605,
    6606,6607,67,6701,6702,6703,6704,6705,6706,6707,68,6801,6802,6803,6804,6805,6806,6807,6808,6809,
    6810,6811,6812,6813,6814,6815,6816,6817,6818,6819,6820,69,6901,6902,6903,6904,6905,6906,6907,70,
    71,7101,7102,7103,7104,7105,7106,7107,7108,7109,7110,7111,7112,7113,7114,7115,7116,7117,7118,7119,
    7120,7121,7122,7123,7124,7125,7126,7127,7128,7129,7130,72,7201,7202,7203,7204,7205,7206,7207,7208,
    7209,7210,7211,7212,7213,7214,7215,7216,7217,7218,7219,7220,7221,7222,7223,7224,7225,7226,7227,7228,
    7229,7230,73,7301,7302,7303,7304,7305,7306,7307,7308,7309,7310,7311,7312,7313,7314,7315,7316,7317,
    7318,7319,7320,7321,7322,7323,7324,7325,7326,7327,7328,7329,7330,74,7401,7402,7403,7404,7405,7406,
    7407,7408,7409,7410,7411,7412,7413,7414,7415,7416,7417,7418,7419,7420,7421,7422,7423,7424,7425,7426,
    7427,7428,7429,7430,75,7501,7502,7503,7504,7505,7506,7507,7508,7509,7510,7511,7512,7513,7514,7515,
    7516,7517,7518,7519,7520,7521,7522,7523,7524,7525,7526,7527,7528,7529,7530,76,7601,7602,7603,7604,
    7605,7606,7607,7608,7609,7610,7611,7612,7613,7614,7615,7616,7617,7618,7619,7620,7621,7622,7623,7624,
    7625,7626,7627,7628,7629,7630,77,7701,7702,7703,7704,7705,7706,7707,7708,7709,7710,7711,7712,7713,
    7714,7715,7716,7717,7718,7719,7720,7721,7722,7723,7724,7725,7726,7727,7728,7729,7730,78,7801,7802,
    7803,7804,7805,7806,7807,7808,7809,7810,7811,7812,7813,7814,7815,7816,7817,7818,7819,7820,7821,7822,
    7823,7824,7825,7826,7827,7828,7829,7830,79,7901,7902,7903,7904,7905,7906,7907,7908,7909,7910,7911,
    7912,7913,7914,7915,7916,7917,7918,7919,7920,7921,7922,7923,7924,7925,7926,7927,7928,7929,7930,80,
    8001,8002,8003,8004,8005,8006,8007,8008,8009,8010,8011,8012,8013,8014,8015,8016,8017,8018,8019,8020,
    8021,8022,8023,8024,8025,8026,8027,8028,8029,8030,81,8101,8102,8103,8104,8105,8106,8107,8108,8109,
    8110,8111,8112,8113,8114,8115,8116,8117,8118,8119,8120,8121,8122,8123,8124,8125,8126,8127,8128,8129,
    8130,82,8201,8202,8203,8204,8205,8206,8207,83,8301,8302,8303,8304,8305,8306,8307,8308,8309,8310,
    8311,8312,8313,8314,84,8401,8402,8403,8404,8405,8406,8407,85,8501,8502,8503,8504,8505,8506,8507,
    8508,8509,8510,8511,8512,8513,8514,8515,8516,8517,8518,8519,8520,86,8601,8602,8603,8604,8605,8606,
    8607,8608,8609,8610,8611,8612,8613,8614,87,8701,8702,8703,8704,8705,8706,8707,8708,8709,8710,8711,
    8712,8713,8714,90,93,95,97,98,99,9000,9001,9002,9003,9004,9005,9006,9007,9008,9009,9010,
    621,
};


void cyclicStaticMapTransfer() {
    char buf[1024];
    if (--ticks_until_move>0) return;
    stopMoving();
    ticks_until_move = 25;
    if (++static_map == ARRAY_SIZE(smlist))
        static_map = 0;
    sprintf(buf, "mapmove %d", smlist[static_map]);
    commAddInput(buf);
}

void randomStaticMapTransfer() {
    int new_static_map;
    char buf[1024];
    if (--ticks_until_move>0) return;
    stopMoving();
    ticks_until_move=25;
    do {
        new_static_map = randInt2(ARRAY_SIZE(smlist));
    } while (new_static_map==static_map);
    static_map = new_static_map;
    sprintf(buf, "mapmove %d", smlist[static_map]);
    commAddInput(buf);
}

void randomStaticMapStressTransfer() {
    int new_static_map;
    char buf[1024];
    if (--ticks_until_move>0) return;
    stopMoving();
    ticks_until_move=25;
    do {
        new_static_map = randInt2(ARRAY_SIZE(static_map_list_all));
    } while (new_static_map==static_map);
    static_map = new_static_map;
    sprintf(buf, "mapmove %d", static_map_list_all[static_map]);
    commAddInput(buf);
}

void testRandomDisconnect(TestClientStage stage, char *stagename, char *file, int line) {
    extern NetLink db_comm_link;
    if (!(g_testMode & TEST_RANDOM_DISCONNECT))
        return;
    if (drop_stage==-1)
        drop_stage = randInt2(MAX_TCS+1);
    if (stage>=drop_stage) {
        printf("\n\nRandom disconnect on stage %d (%s), file %s (%d)\n", stage, stagename, file, line);
        statusUpdate("RandDiscon");
        lnkFlushAll();
        // free all the links/close all the sockets here?
        clearNetLink(&db_comm_link);
        clearNetLink(&comm_link);
        packetShutdown();
        sendMessageToLauncher("RandDiscon: %2d (%s)", stage, stagename);
        if (g_testMode & TEST_TIMED_RECONNECT) {
            statusUpdate("Reconnect");
            Sleep(5000);
            //exit(-1);
        }
        error_exit(0);
    }
}


void mapSelectReceiveText(Packet* pak)
{
    char* text;
    char cmdList[50][200];
    int cmdCount = 0;
    char* cmdBegin;

    pktGetF32(pak);
    pktGetF32(pak);
    pktGetF32(pak);

    text = pktGetString(pak);

    cmdBegin = text;

    while(cmdBegin = strstri(cmdBegin, "cmd:"))
    {
        char* cmdEnd = strstri(cmdBegin + 4, "'");

        if(!cmdEnd || cmdCount >= ARRAY_SIZE(cmdList))
            break;

        cmdBegin += 4;

        if(cmdEnd - cmdBegin < ARRAY_SIZE(cmdList[0]) - 1)
        {
            int cmdLength = cmdEnd - cmdBegin;

            memcpy(cmdList[cmdCount], cmdBegin, cmdLength);

            cmdList[cmdCount][cmdLength] = 0;

            cmdCount++;
        }

        cmdBegin = cmdEnd;
    }

    if(cmdCount)
    {
        int cmdToRun = rand() % cmdCount;
        Vec3 pos;
        char* cmdName;
        int map_id;

        printf("Running door command: %s\n", cmdList[cmdToRun]);

        beginParse(cmdList[cmdToRun]);
        getString(&cmdName);
        getFloat(&pos[0]);
        getFloat(&pos[1]);
        getFloat(&pos[2]);
        getInt(&map_id);
        endParse();

        START_INPUT_PACKET(pak, CLIENTINP_ENTER_DOOR );
        pktSendF32( pak, pos[0] );
        pktSendF32( pak, pos[1] );
        pktSendF32( pak, pos[2] );
        pktSendBitsPack( pak, 1, map_id );
        END_INPUT_PACKET
    }
}
