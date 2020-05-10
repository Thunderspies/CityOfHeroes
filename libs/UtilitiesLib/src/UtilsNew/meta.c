#include "utilitieslib/stdtypes.h"
#include "utilitieslib/UtilsNew/meta.h"

#include "utilitieslib/utils/utils.h"
#include "utilitieslib/utils/error.h"
#include "utilitieslib/network/netio.h"
#include "utilitieslib/components/EString.h"
#include "utilitieslib/components/StashTable.h"
#include "utilitieslib/UtilsNew/Array.h"
#include "utilitieslib/UtilsNew/Str.h"

#include "utilitieslib/utils/cmdoldparse.h"
#include "utilitieslib/language/MessageStore.h"
#include "utilitieslib/language/MessageStoreUtil.h"

typedef void (*function_call)();

__declspec(thread) CallInfo *gthread_metacall_stack;
CallInfo g_metacall_null = {0};

static StashTable s_calllookup[CALLTYPE_COUNT]; // FIXME: thread safety
static Cmd *s_legacycmds = NULL; // not thread-safe, not updated as commands change
static vprintf_call s_remote_printf_messager;
static vprintf_call s_remote_handler_messager;

FORCEINLINE static int s_stripUnderscores(const char *cmd, char **buffer)
{
    const char *s;

    if(!cmd)
        return 0;

    Str_clear(buffer);
    for(s = cmd; *s; s++)
        if(*s != '_')
            Str_catf(buffer, "%c", *s);

    return 1;
}


void meta_registerCall(CallType calltype, ObjectInfo *object, const char *name)
{
    char *name_buf = Str_temp();
    assert(INRANGE0(calltype, CALLTYPE_COUNT));
    if(!s_calllookup[calltype])
        s_calllookup[calltype] = stashTableCreateWithStringKeys(0, StashDeepCopyKeys);

    // FIXME: copy objects?
    // FIXME: better collision handling
    // FIXME: verify object

    if(calltype == CALLTYPE_CONSOLE)
    {
        Cmd *cmd;

        if(s_legacycmds)
            as_pop(&s_legacycmds); // null terminator

        cmd = as_push(&s_legacycmds);
        cmd->access_level = object->accesslevel;
        cmd->name = strdup(name); // with underscores
        cmd->comment = object->console_desc ? object->console_desc : object->name;

        (void)as_push(&s_legacycmds); // null terminator

        // the actual lookup name ignores underscores
        s_stripUnderscores(name, &name_buf);
        name = name_buf;
    }

    assert(stashAddPointer(s_calllookup[calltype], name, object, false));
    Str_destroy(&name_buf);
}

void meta_registerPrintfMessager(vprintf_call messager)
{
    s_remote_printf_messager = messager;
}

void meta_registerRemoteMessager(vprintf_call messager)
{
    s_remote_handler_messager = messager;
}

void meta_printf(const char *fmt, ...)
{
    CallInfo *caller = meta_getCallInfo();

    VA_START(va, fmt);
    switch(caller ? caller->calltype : CALLTYPE_NULL)
    {
        xcase CALLTYPE_NULL:

        xcase CALLTYPE_LOCAL:

        xcase CALLTYPE_COMMANDLINE:
            vprintf(fmt, va);

        xcase CALLTYPE_REMOTE:
            if(s_remote_printf_messager)
            {
                s_remote_printf_messager(fmt, va);
            }
            else
            {
                static int s_warned = 0;
                if(!s_warned)
                    printf( "Warning: unhandled caller_printf type %d", caller->calltype);
                printf( fmt, va);
            }

        xdefault:
            printf("Warning: unhandled caller_printf type %d", caller->calltype);
    }
    VA_END();
}

static int s_getValue_string(TypeInfo *typeinfo, char *str, intptr_t *ret)
{
    switch(typeinfo->type)
    {
        xcase TYPE_INTEGER:
            *ret = atoi(str);

        xcase TYPE_FLOATINGPOINT:
            *(F32*)ret = atof(str);

        xcase TYPE_STRING:
            *(char**)ret = str;

        xdefault:
            FatalErrorf("Unhandled command line parameter type");
    }

    return 1; // FIXME: parameter checking
}

static void s_getValue_packet(TypeInfo *typeinfo, Packet *pak_in, intptr_t *ret)
{
    switch(typeinfo->type)
    {
        xcase TYPE_INTEGER:
            *ret = pktGetBitsAuto(pak_in);

        xcase TYPE_FLOATINGPOINT:
            *(F32*)ret = pktGetF32(pak_in);

        xcase TYPE_STRING:
            *(char**)ret = pktGetStringTemp(pak_in);

        xdefault:
            FatalErrorf("Unhandled packet parameter type");
    }
}

void meta_handleCommandLine(int argc, char **argv)
{
    int i;
    ObjectInfo *object;

    THREADSAFE_STATIC_MARK(gthread_metacall_stack);

    as_push(&gthread_metacall_stack)->calltype = CALLTYPE_COMMANDLINE;

    for(i = 1; i < argc; i++)
    {
        if(    !argv[i] || argv[i][0] != '-' ||
            !stashFindPointer(s_calllookup[CALLTYPE_COMMANDLINE], argv[i]+1, &object))
        {
            printf("Warning: Meta Invalid parameter %s", argv[i]);
            continue;
        }

        if(!object)
            continue;

        switch(object->typeinfo.type)
        {
            xcase TYPE_INTEGER:
                if(i+1 < argc && argv[i+1][0] != '-')
                    *(int*)object->location = atoi(argv[++i]);
                else
                    (*(int*)object->location)++;

            xcase TYPE_FLOATINGPOINT:
                if(i+1 < argc && argv[i+1][0] != '-')
                    *(F32*)object->location = atof(argv[++i]);
                else
                    (*(F32*)object->location)++;

            xcase TYPE_STRING:
                SAFE_FREE(object->location);
                if(i+1 < argc && argv[i+1][0] != '-')
                    object->location = strdup(argv[++i]);

            xcase TYPE_FUNCTION:
                if(i + as_size(&object->typeinfo.function_parameters) >= argc)
                {
                    printf("Warning: Meta Invalid usage of %s", argv[i]);
                }
                else
                {
                    int j, ret = 1;
                    intptr_t params[4] = {0};
                    assert(as_size(&object->typeinfo.function_parameters) <= 4);
                    for(j = 0; ret && j < as_size(&object->typeinfo.function_parameters); j++)
                        ret &= s_getValue_string(&object->typeinfo.function_parameters[j].typeinfo, argv[++i], &params[j]);
                    if(ret)
                        ((function_call)object->location)(params[0],params[1],params[2],params[3]);
                }

            xdefault:
                FatalErrorf("Unhandled command line parameter type");
        }
    }

    as_pop(&gthread_metacall_stack);
}

Cmd* meta_getLegacyCmds(void)
{
    if(!s_legacycmds)
        (void)as_push(&s_legacycmds); // null terminator
    return s_legacycmds;
}

static void s_tokenize(char ***ptoks, char *command)
{
    int quote = 0;
    char *s = command;
    while(s[0])
    {
        if(!quote)
        {
            while(s[0] == ' ')
                s++;

            if(s[0] == '"')
            {
                s++;
                quote = 1;
            }
        }

        if(s[0])
            ap_push(ptoks, s);

        while(s[0] && (quote || s[0] != ' ') && s[0] != '"')
            s++;

        if(s[0])
        {
            if(s[0] == '"')
                quote = !quote;
            (s++)[0] = '\0';
        }
    }
}


int meta_handleConsole(const char *command, int forward_id, int accesslevel, char *output, int output_len)
{
    ObjectInfo *object;
    int ret;
    int plus = 0;

    char **toks = (char**)ap_temp();
    char *buf = achr_temp();
    char *nameBuf = Str_temp();

    CallInfo *caller = as_push(&gthread_metacall_stack);
    THREADSAFE_STATIC_MARK(gthread_metacall_stack);

    caller->calltype = CALLTYPE_CONSOLE;
    caller->callflags = CALLFLAG_CONSOLE;
    caller->forward_id = forward_id;

    for(; command[0] == '+'; command++)
        plus++;
    for(; command[0] == '-'; command++)
        plus--;

    achr_ncp(&buf, command, strlen(command) + 1);
    s_tokenize(&toks, buf);
    if(ap_size(&toks))
        s_stripUnderscores(toks[0], &nameBuf);

    assert(output_len);
    output[0] = '\0';

    if(!ap_size(&toks))
    {
        // no command
        ret = 1;
    }
    else if(!stashFindPointer(s_calllookup[CALLTYPE_CONSOLE], nameBuf, &object))
    {
        msPrintf(menuMessages, output, output_len, "UnknownCommandString", toks[0]);
        ret = -1;
    }
    else if(!object)
    {
        // null handler
        ret = 1;
    }
    else if(object->accesslevel > accesslevel)
    {
        msPrintf(menuMessages, output, output_len, "UnknownCommandString", toks[0]);
        ret = 0;
    }
    else
    {
        switch(object->typeinfo.type)
        {
            xcase TYPE_INTEGER:
                if(ap_size(&toks) == 1)
                {
                    if(plus > 1)
                        *(int*)object->location = !*(int*)object->location;
                    if(plus == 1)
                        *(int*)object->location = 1;
                    else if(plus < 0)
                        *(int*)object->location = 0;

                    sprintf_s(output, output_len, "%s %d", toks[0], *(int*)object->location);
                    ret = 1;
                }
                else if(ap_size(&toks) == 2)
                {
                    *(int*)object->location = atoi(toks[1]);

                    sprintf_s(output, output_len, "%s %d", toks[0], *(int*)object->location);
                    ret = 1;
                }
                else
                {
                    msPrintf(menuMessages, output, output_len, "IncorrectUsage", toks[0], 1, ap_size(&toks)-1, object->console_desc ? object->console_desc : object->name);
                    strcatf_s(output, output_len, "\n %s <int>", toks[0]);
                    ret = 0;
                }

            xcase TYPE_FLOATINGPOINT:
                if(ap_size(&toks) == 1)
                {
                    if(plus > 1)
                        *(F32*)object->location = !*(int*)object->location;
                    if(plus == 1)
                        *(F32*)object->location = 1.f;
                    else if(plus < 0)
                        *(F32*)object->location = 0.f;

                    sprintf_s(output, output_len, "%s %f", toks[0], *(F32*)object->location);
                    ret = 1;
                }
                else if(ap_size(&toks) == 2)
                {
                    *(F32*)object->location = atof(toks[1]);

                    sprintf_s(output, output_len, "%s %f", toks[0], *(F32*)object->location);
                    ret = 1;
                }
                else
                {
                    msPrintf(menuMessages, output, output_len, "IncorrectUsage", toks[0], 1, ap_size(&toks)-1, object->console_desc ? object->console_desc : object->name);
                    strcatf_s(output, output_len, "\n %s <float>", toks[0]);
                    ret = 0;
                }

            xcase TYPE_STRING:
                if(ap_size(&toks) == 1)
                {
                    sprintf_s(output, output_len, "%s %s", toks[0], *(char**)object->location);
                    ret = 1;
                }
                else if(ap_size(&toks) == 2)
                {
                    SAFE_FREE(*(char**)object->location);
                    *(char**)object->location = strdup(toks[1]);

                    sprintf_s(output, output_len, "%s %s", toks[0], *(char**)object->location);
                    ret = 1;
                }
                else
                {
                    msPrintf(menuMessages, output, output_len, "IncorrectUsage", toks[0], 1, ap_size(&toks)-1, object->console_desc ? object->console_desc : object->name);
                    strcatf_s(output, output_len, "\n %s <string>", toks[0]);
                    ret = 0;
                }

            xcase TYPE_FUNCTION:
                if(    ap_size(&toks)-1 >= object->console_reqparams &&
                    ap_size(&toks)-1 <= as_size(&object->typeinfo.function_parameters) )
                {
                    int i;
                    intptr_t params[4];
                    assert(as_size(&object->typeinfo.function_parameters) <= 4);
                    ret = 1;
                    for(i = 0; ret && i < as_size(&object->typeinfo.function_parameters); i++)
                    {
                        if(i+1 < ap_size(&toks))
                            ret &= s_getValue_string(&object->typeinfo.function_parameters[i].typeinfo, toks[i+1], &params[i]);
                        else
                            params[i] = 0;
                    }
                    if(ret)
                        ((function_call)object->location)(params[0],params[1],params[2],params[3]);
                }
                else
                {
                    ret = 0;
                }
                if(!ret)
                {
                    int i;
                    msPrintf(menuMessages, output, output_len, "IncorrectUsage", toks[0], object->console_reqparams, ap_size(&toks)-1, object->console_desc ? object->console_desc : object->name);
                    strcatf_s(output, output_len, "\n %s", toks[0]);
                    for(i = 0; i < as_size(&object->typeinfo.function_parameters); i++)
                    {
                        switch(object->typeinfo.function_parameters[i].typeinfo.type)
                        {
                            xcase TYPE_INTEGER:            strcatf_s(output, output_len, i < object->console_reqparams ? " <int %s>" : " [int %s]", object->typeinfo.function_parameters[i].name);
                            xcase TYPE_FLOATINGPOINT:    strcatf_s(output, output_len, i < object->console_reqparams ? " <float %s>" : " [float %s]", object->typeinfo.function_parameters[i].name);
                            xcase TYPE_STRING:            strcatf_s(output, output_len, i < object->console_reqparams ? " <string %s>" : " [string %s]", object->typeinfo.function_parameters[i].name);
                            xdefault:                    FatalErrorf("Unhandled command line parameter type");
                        }
                    }
                    ret = 0;
                }

            xdefault:
                FatalErrorf("Unhandled console type");
                ret = 0;
        }
    }

    ap_destroy(&toks, NULL);
    achr_destroy(&buf);
    Str_destroy(&nameBuf);
    as_pop(&gthread_metacall_stack);
    return ret;
}

void meta_pktSendRoute(Packet *pak_out)
{
    int i;
    CallInfo *caller = meta_getCallInfo();
    pktSendBitsAuto(pak_out, caller->callflags);
    pktSendBitsAuto(pak_out, caller->route_steps + !!caller->forward_id);
    for(i = 0; i < caller->route_steps; i++)
        pktSendBitsAuto(pak_out, caller->route_ids[i]);
    if(caller->forward_id)
        pktSendBitsAuto(pak_out, caller->forward_id);
}

static void s_remote_printf(const char *fmt, ...)
{
    VA_START(va, fmt);
    if(s_remote_printf_messager)
        s_remote_printf_messager(fmt, va);
    else
        vprintf(fmt, va);
    VA_END();
}

void meta_handleRemote(NetLink *link, Packet *pak_in, int forward_id, int accesslevel)
{
    char *function_name;
    ObjectInfo *object;
    int i;

    CallInfo *caller = as_push(&gthread_metacall_stack);
    THREADSAFE_STATIC_MARK(gthread_metacall_stack);

    caller->calltype = CALLTYPE_REMOTE;
    caller->link = link;
    caller->callflags = pktGetBitsAuto(pak_in);
    caller->route_steps = pktGetBitsAuto(pak_in);
    if(caller->route_steps + !!forward_id > MAX_ROUTE_STEPS)
    {
        printf("Warning: Meta remote call route list overflow (%d steps).", caller->route_steps);
        as_pop(&gthread_metacall_stack);
        return;
    }
    for(i = 0; i < caller->route_steps; i++)
        caller->route_ids[i] = pktGetBitsAuto(pak_in);
    caller->forward_id = forward_id;

    function_name = pktGetStringTemp(pak_in);
    if(    !function_name || !function_name[0] ||
        !stashFindPointer(s_calllookup[CALLTYPE_REMOTE], function_name, &object) )
    {
        s_remote_printf("Warning: Invalid remote call %s", function_name);
    }
    else if(!object)
    {
        // null handler, do nothing
    }
    else if(object->accesslevel > accesslevel)
    {
        s_remote_printf("Warning: Insufficient accesslevel for %s (got %d, need %d)", function_name, accesslevel, object->accesslevel);
    }
    else if(object->receivepacket)
    {
        // custom handler
        object->receivepacket(pak_in);
    }
    else
    {
        // dynamic handler
        switch(object->typeinfo.type)
        {
            xcase TYPE_INTEGER:
                *(int*)object->location = pktGetBitsAuto(pak_in);

            xcase TYPE_FLOATINGPOINT:
                *(F32*)object->location = pktGetF32(pak_in);

            xcase TYPE_STRING:
                SAFE_FREE(object->location);
                object->location = strdup(pktGetStringTemp(pak_in)); // FIXME: allow nulls

            xcase TYPE_FUNCTION:
            {
                int j;
                intptr_t params[4] = {0};
                assert(as_size(&object->typeinfo.function_parameters) <= 4);
                for(j = 0; j < as_size(&object->typeinfo.function_parameters); j++)
                    s_getValue_packet(&object->typeinfo.function_parameters[j].typeinfo, pak_in, &params[j]);
                ((function_call)object->location)(params[0],params[1],params[2],params[3]);
            }

            xdefault:
                FatalErrorf("Unhandled command line parameter type");
        }
    }

    as_pop(&gthread_metacall_stack);
}
