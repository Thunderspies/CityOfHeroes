#pragma once

#include <stdint.h>

#define    META(...)        // put this ahead of your function or global variable declaration
#define        ACCESSLEVEL    // sets the accesslevel for user-initiated commands
#define        COMMANDLINE    // create a command line parameter, use COMMANDLINE(name) to specify a name
#define        CONSOLE        // create a in-game console command, use CONSOLE(name) to specify a name
                        //    console commands will automatically propagate to higher servers
                        //    a second command like project_name will be created as an easy way around collisions
                        //    e.g. if you use CONSOLE(foo) on the mission server, you'll get foo and missionserver_foo commands
#define            CONSOLE_DESC        // provides a summary for use command listings
#define            CONSOLE_REQPARAMS    // this is a hack until I put in per-parameter markup
#define        REMOTE        // create a remote procedure call
// #define        LOADER        // create a background loader command


#define MAX_ROUTE_STEPS        8

typedef enum TypeType
{
    TYPE_VOID,
    TYPE_INTEGER,
    TYPE_FLOATINGPOINT,
    TYPE_STRING,
    TYPE_STRUCT,
    TYPE_FUNCTION,
    TYPE_COUNT
} TypeType;

typedef struct Packet Packet;
typedef void (*ReceivePacket)(Packet*);
typedef int (*vprintf_call)(const char *fmt, void *va_args); // currently, the return value isn't used, but non-negative should be considered success

typedef struct TypeInfo
{
    TypeType    type;
    size_t        size;
    size_t        count;    // 0 for managed arrays, 1 for single objects, >1 for embedded arrays

    intptr_t    primitive_default;
    unsigned    primitive_signed    : 1;

    struct TypeMember    *struct_members;
    char                *struct_typename;

    struct TypeParameter    *function_parameters;

    char    *filename;
    int        fileline;

} TypeInfo;

typedef struct TypeMember
{
    char *name;
    size_t offset;
    TypeInfo typeinfo;
} TypeMember;

typedef struct TypeParameter
{
    char *name;
    TypeInfo typeinfo;
} TypeParameter;

typedef struct ObjectInfo
{
    char *name;
    TypeInfo typeinfo;

    int accesslevel;
    char *console_desc;
    int console_reqparams;

    void *location;

    char *filename;
    int fileline;

    ReceivePacket receivepacket;

} ObjectInfo;

typedef enum CallType
{
    CALLTYPE_NULL, // FIXME: use -1 for this? remove this?
    CALLTYPE_LOCAL,
    CALLTYPE_COMMANDLINE,
    CALLTYPE_CONSOLE,
    CALLTYPE_REMOTE,
//     CALLTYPE_LOADER,
    CALLTYPE_COUNT
} CallType;

typedef enum CallFlags
{
    CALLFLAG_CONSOLE    = (1<<0), // came from a console at some point
} CallFlags;

typedef struct Cmd Cmd;
typedef struct NetLink NetLink;
typedef struct Packet Packet;

typedef struct CallInfo
{
    CallType calltype;
    CallFlags callflags;

    // remote
    NetLink *link;
    int route_steps;
    int route_ids[MAX_ROUTE_STEPS];
    int forward_id;

} CallInfo;

void meta_registerCall(CallType type, ObjectInfo *object, const char *name);
void meta_registerPrintfMessager(vprintf_call messager);    // for meta_printf when handling remote calls
void meta_registerRemoteMessager(vprintf_call messager);    // for warnings etc. when handling remote calls

void meta_handleCommandLine(int argc, char **argv);
Cmd* meta_getLegacyCmds(void);
int meta_handleConsole(const char *command, int forward_id, int accesslevel, char *output, int output_len); // returns success.
void meta_handleRemote(NetLink *link, Packet *pak_in, int forward_id, int accesslevel);

void meta_pktSendRoute(Packet *pak_out);
#ifndef FORMAT
#define FORMAT const char*
#endif
void meta_printf(FORMAT fmt, ...);

// Meta System State Management ///////////////////////////////////////////////

#include "utilitieslib/UtilsNew/Array.h"

extern __declspec(thread) CallInfo *gthread_metacall_stack;
extern CallInfo g_metacall_null;

#define meta_getCallInfo()    (as_size(&gthread_metacall_stack) ? as_top(&gthread_metacall_stack) : &g_metacall_null)

#define meta_local(call)                                            \
(void)(                                                                \
    as_push(&gthread_metacall_stack)->calltype = CALLTYPE_LOCAL,    \
    (call),                                                            \
    as_pop(&gthread_metacall_stack)                                    \
)

#define meta_remote(link_, forward_id_, call)                        \
(void)(                                                                \
    as_push(&gthread_metacall_stack),                                \
    as_top(&gthread_metacall_stack)->calltype = CALLTYPE_REMOTE,    \
    as_top(&gthread_metacall_stack)->link = (link_),                \
    as_top(&gthread_metacall_stack)->forward_id = (forward_id_),    \
    (call),                                                            \
    as_pop(&gthread_metacall_stack)                                    \
)

#define meta_console(link_, forward_id_, call)                        \
(void)(                                                                \
    as_push(&gthread_metacall_stack),                                \
    as_top(&gthread_metacall_stack)->calltype = CALLTYPE_REMOTE,    \
    as_top(&gthread_metacall_stack)->link = (link_),                \
    as_top(&gthread_metacall_stack)->forward_id = (forward_id_),    \
    as_top(&gthread_metacall_stack)->callflags = CALLFLAG_CONSOLE,    \
    (call),                                                            \
    as_pop(&gthread_metacall_stack)                                    \
)
