#include "autoCommands.h"

#include "dbcomm/dbcontainer.h"
#include "container/dbcontainerpack.h"
#include <utilitieslib/components/earray.h>
#include <utilitieslib/components/MemoryPool.h>
#include "comm_backend.h"
#include "dbcomm/dbcomm.h"
#include "entity/entVarUpdate.h"
#include <utilitieslib/utils/mathutil.h>
#include "entity/entity.h"
#include <utilitieslib/utils/timing.h>
#include "cmdparse/cmdserver.h"
#include <utilitieslib/utils/strings_opt.h>
#include "gameComm/sendToClient.h"

AutoCommand **g_autoCommands = 0;
#define MAX_AUTO_COMMAND_COUNT 100
#define WARN_AUTO_COMMAND_COUNT 90

LineDesc autocommand_line_desc[] =
{
    {{ PACKTYPE_INT,            SIZE_INT32,                            "ExecutionTime",    OFFSET(AutoCommand, executionTime)    },
            "The time at which the AutoCommand begins to run on players that are logging in." },
    {{ PACKTYPE_STR_UTF8,            SIZEOF2(AutoCommand, command),        "Command",            OFFSET(AutoCommand, command)        },
            "The slash command (server-only) that the AutoCommand runs on players that are logging in." },
    {{ PACKTYPE_STR_UTF8,            SIZEOF2(AutoCommand, creator),        "Creator",            OFFSET(AutoCommand, creator)        },
            "The authname of the person that created this AutoCommand." },
    {{ PACKTYPE_INT,            SIZE_INT32,                            "CreationTime",        OFFSET(AutoCommand, creationTime)    },
            "The time at which this AutoCommand was created." },
    {{ (ContainerPackType)0 }},
};

StructDesc autocommand_desc[] =
{
    sizeof(AutoCommand),
    {AT_NOT_ARRAY,{0}},
    autocommand_line_desc,
    ""
};

MP_DEFINE(AutoCommand);

AutoCommand* AutoCommandCreate(void)
{
    MP_CREATE(AutoCommand, MAX_AUTO_COMMAND_COUNT);
    return MP_ALLOC(AutoCommand);
}

void AutoCommandDestroy(AutoCommand* autoCommand)
{
    MP_FREE(AutoCommand, autoCommand);
}

static AutoCommand* autoCommand_findCommandById(int id)
{
    int commandIndex, commandCount;

    commandCount = eaSize(&g_autoCommands);

    for (commandIndex = 0; commandIndex < commandCount; commandIndex++)
    {
        if (g_autoCommands[commandIndex]->id == id)
        {
            return g_autoCommands[commandIndex];
        }
    }

    return NULL;
}

static int autoCommand_findIndexByTime(int timestamp)
{
    int commandIndex, commandCount;

    commandCount = eaSize(&g_autoCommands);
    for (commandIndex = 0; commandIndex < commandCount; commandIndex++)
    {
        if (g_autoCommands[commandIndex]->executionTime >= timestamp)
            break;
    }

    return commandIndex; // can return equal to commandCount if the timestamp is past the last
}

static AutoCommand* containerHandleAutoCommand_removeFromList(int id)
{
    AutoCommand *existingAutoCommand = NULL;
    int commandIndex, commandCount;

    commandCount = eaSize(&g_autoCommands);
    for (commandIndex = 0; commandIndex < commandCount; commandIndex++)
    {
        if (g_autoCommands[commandIndex]->id == id)
        {
            existingAutoCommand = g_autoCommands[commandIndex];
            eaRemove(&g_autoCommands, commandIndex);
            return existingAutoCommand;
        }
    }

    return NULL;
}

static void containerHandleAutoCommand_deleteFromList(AutoCommand *tempAutoCommand)
{
    AutoCommand *existingAutoCommand = NULL;

    if (!tempAutoCommand)
        return;

    existingAutoCommand = containerHandleAutoCommand_removeFromList(tempAutoCommand->id);

    if (existingAutoCommand)
    {
        AutoCommandDestroy(existingAutoCommand);
    }
}

static void containerHandleAutoCommand_addToOrModifyInList(AutoCommand *tempAutoCommand)
{
    AutoCommand *existingAutoCommand = NULL;
    int commandIndex;

    if (!tempAutoCommand)
        return;

    existingAutoCommand = containerHandleAutoCommand_removeFromList(tempAutoCommand->id);

    if (!existingAutoCommand)
    {
        existingAutoCommand = AutoCommandCreate();
        existingAutoCommand->id = tempAutoCommand->id;
    }

    existingAutoCommand->executionTime = tempAutoCommand->executionTime;
    strncpy(existingAutoCommand->command, tempAutoCommand->command, AUTO_COMMAND_STRING_LENGTH);
    strncpy(existingAutoCommand->creator, tempAutoCommand->creator, sizeof(tempAutoCommand->creator) / sizeof(tempAutoCommand->creator[0]));
    existingAutoCommand->creationTime = tempAutoCommand->creationTime;

    commandIndex = autoCommand_findIndexByTime(tempAutoCommand->executionTime);
    eaInsert(&g_autoCommands, existingAutoCommand, commandIndex);
}

void containerHandleAutoCommand(ContainerInfo *ci)
{
    AutoCommand tempAutoCommand = {0};

    if (!ci)
        return;

    dbContainerUnpack(autocommand_desc,ci->data,(char*)&tempAutoCommand);
    tempAutoCommand.id = ci->id;

    if (ci->delete_me)
    {
        containerHandleAutoCommand_deleteFromList(&tempAutoCommand);
    }
    else
    {
        containerHandleAutoCommand_addToOrModifyInList(&tempAutoCommand);
    }
};

void autoCommand_add(Entity *creator, char *commandName, int days, int hours, int minutes)
{
    AutoCommand newCommand = {0};
    int now = dbSecondsSince2000();
    char dateString[AUTO_COMMAND_STRING_LENGTH];

    if (!creator || !commandName || !creator->auth_name)
        return;

    if (eaSize(&g_autoCommands) >= MAX_AUTO_COMMAND_COUNT)
    {
        // This functionality is added primarily as a check to ensure this system isn't being used
        // more prolifically than we intend.  If this gets hit, we should consider refactoring
        // the system to ensure this doesn't start causing performance issues.
        sendInfoBox(creator, INFO_SVR_COM, "CantAddAutoCommandTooManyInSystem");
    }
    else if (days < 0 || hours < 0 || hours >= 24 || minutes < 0 || minutes >= 60)
    {
        sendInfoBox(creator, INFO_SVR_COM, "CantAddAutoCommandBadTimeParameters");
    }
     else if (!days && !hours && creator->access_level <= 9)
     {
         // not allowed to add Auto Commands closer than one hour
         sendInfoBox(creator, INFO_SVR_COM, "CantAddAutoCommandSoonerThanOneHour");
     }
    else
    {
        newCommand.executionTime = now + ((days * 24 + hours) * 60 + minutes) * 60;
        strncpy(newCommand.command, commandName, MIN(strlen(commandName), AUTO_COMMAND_STRING_LENGTH));
        strncpy(newCommand.creator, creator->auth_name, MIN(strlen(creator->auth_name), sizeof(creator->auth_name) / sizeof(creator->auth_name[0])));
        newCommand.creationTime = now;
        dbSyncContainerUpdate(CONTAINER_AUTOCOMMANDS, -1, CONTAINER_CMD_CREATE, dbContainerPackage(autocommand_desc,(char*)&newCommand));

        timerMakeDateStringFromSecondsSince2000(dateString, newCommand.executionTime);
        sendInfoBox(creator, INFO_SVR_COM, "AutoCommandAdded");
        dbLog("AutoCommand:Add", creator, "Added command %s to run at %s", newCommand.command, dateString);
        if (eaSize(&g_autoCommands) >= WARN_AUTO_COMMAND_COUNT)
        {
            sendInfoBox(creator, INFO_SVR_COM, "WarningAutoCommandsNearingLimit");
        }
    }
}

void autoCommand_delete(Entity *deleter, int id)
{
    int now = dbSecondsSince2000();
    AutoCommand *autoCommand;
    char dateString[AUTO_COMMAND_STRING_LENGTH];

    if (!deleter)
        return;

    autoCommand = autoCommand_findCommandById(id);

    if (!autoCommand || autoCommand->id != id)
    {
        sendInfoBox(deleter, INFO_SVR_COM, "AutoCommandNotFound");
    }
    else if (autoCommand->executionTime - now < 30 * 60 && deleter->access_level <= 9)
    {
        sendInfoBox(deleter, INFO_SVR_COM, "CantDeleteAutoCommandSoonerThanHalfHour");
    }
    else
    {
        dbSyncContainerUpdate(CONTAINER_AUTOCOMMANDS, id, CONTAINER_CMD_DELETE, (char*)autoCommand);
        sendInfoBox(deleter, INFO_SVR_COM, "AutoCommandDeleted");
        timerMakeDateStringFromSecondsSince2000(dateString, autoCommand->executionTime);
        dbLog("AutoCommand:Delete", deleter, "Deleted command %s that would have run at %s", autoCommand->command, dateString);
    }
}

void autoCommand_showAll(Entity *user)
{
    int autoCommandIndex, autoCommandCount;
    int now = dbSecondsSince2000();
    char dateString[AUTO_COMMAND_STRING_LENGTH];

    if (!user)
        return;

    sendInfoBox(user, INFO_SVR_COM, "ListingAllAutoCommands");

    autoCommandCount = eaSize(&g_autoCommands);
    for (autoCommandIndex = 0; autoCommandIndex < autoCommandCount; autoCommandIndex++)
    {
        AutoCommand *autoCommand = g_autoCommands[autoCommandIndex];
        
        if (!autoCommand)
            continue;

        sendInfoBox(user, INFO_SVR_COM, "%i: %s", autoCommand->id, autoCommand->command);
        
        timerMakeDateStringFromSecondsSince2000(dateString, autoCommand->executionTime);
        if (autoCommand->executionTime - now < 30 * 60)
        {
            sendInfoBox(user, INFO_SVR_COM, "AutoCommandRunsAtCannotDelete", dateString);
        }
        else
        {
            sendInfoBox(user, INFO_SVR_COM, "AutoCommandRunsAtCanDelete", dateString);
        }

        timerMakeDateStringFromSecondsSince2000(dateString, autoCommand->creationTime);
        sendInfoBox(user, INFO_SVR_COM, "AutoCommandAddedBy", autoCommand->creator, dateString);
    }

    sendInfoBox(user, INFO_SVR_COM, "EndListingAllAutoCommands");
}

void autoCommand_showByCommand(Entity *user, char *command)
{
    int autoCommandIndex, autoCommandCount;
    int now = dbSecondsSince2000();
    char dateString[AUTO_COMMAND_STRING_LENGTH];
    char commandLowered[AUTO_COMMAND_STRING_LENGTH];
    char *autoCommandLowered;

    if (!user || !command)
        return;

    sendInfoBox(user, INFO_SVR_COM, "ListingAutoCommandsByCommand", command);

    strcpy(commandLowered, stripUnderscores(command));

    autoCommandCount = eaSize(&g_autoCommands);
    for (autoCommandIndex = 0; autoCommandIndex < autoCommandCount; autoCommandIndex++)
    {
        AutoCommand *autoCommand = g_autoCommands[autoCommandIndex];

        if (!autoCommand || !autoCommand->command)
        {
            continue;
        }

        autoCommandLowered = stripUnderscores(autoCommand->command);

        if (!strstri(autoCommandLowered, commandLowered))
        {
            continue;
        }

        sendInfoBox(user, INFO_SVR_COM, "%i: %s", autoCommand->id, autoCommand->command);

        timerMakeDateStringFromSecondsSince2000(dateString, autoCommand->executionTime);
        if (autoCommand->executionTime - now < 30 * 60)
        {
            sendInfoBox(user, INFO_SVR_COM, "AutoCommandRunsAtCannotDelete", dateString);
        }
        else
        {
            sendInfoBox(user, INFO_SVR_COM, "AutoCommandRunsAtCanDelete", dateString);
        }

        timerMakeDateStringFromSecondsSince2000(dateString, autoCommand->creationTime);
        sendInfoBox(user, INFO_SVR_COM, "AutoCommandAddedBy", autoCommand->creator, dateString);
    }

    sendInfoBox(user, INFO_SVR_COM, "EndListingAutoCommandsByCommand", command);
}

static void autoCommand_runCommand(ClientLink *link, Entity *e, AutoCommand *command, int testRun)
{
    if (!link || !e || !command)
        return;

    if (!testRun)
    {
        dbLog("AutoCommands:Run", e, "Running Auto Command: %s", command->command);
    }
    else
    {
        conPrintf(link, "Running Auto Command: %s that runs at time %s. (Created by %s at %s)", 
                    command->command, timerMakeDateStringFromSecondsSince2000NoSeconds(command->executionTime), 
                    command->creator, timerMakeDateStringFromSecondsSince2000NoSeconds(command->creationTime));
    }

    serverParseClientEx(command->command, link, ACCESS_DEBUG);
}

void autoCommand_run(ClientLink *link, Entity *e, int testRun)
{
    int autoCommandIndex, autoCommandCount;
    int now = dbSecondsSince2000();

    if (!link || !e)
        return;

    autoCommandCount = eaSize(&g_autoCommands);
    autoCommandIndex = autoCommand_findIndexByTime(e->lastAutoCommandRunTime);

    while (autoCommandIndex < autoCommandCount && g_autoCommands[autoCommandIndex]
            && g_autoCommands[autoCommandIndex]->executionTime < now)
    {
        autoCommand_runCommand(link, e, g_autoCommands[autoCommandIndex], testRun);
        autoCommandIndex++;
    }

    e->lastAutoCommandRunTime = now;
}