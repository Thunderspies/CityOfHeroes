#include "dbcomm/dbcomm.h"
#include "comm_backend.h"
#include "container/dbcontainerpack.h"
#include <utilitieslib/components/EString.h>
#include <utilitieslib/utils/utils.h>
#include <stdio.h>
#include <string.h>

#include "base_persistence.inc"

static char *stored;
static int valid = 1;
static int created;

int dbSyncContainerFindByElement(int list, char *field, char *value,
	int *map, int online)
{
	DBBase base = {0};
	valid &= list == CONTAINER_BASE && !map && !online;
	if (!stored)
		return 0;
	dbContainerUnpack(base_desc, stored, &base);
	int owner = !strcmp(field, "SupergroupId") ? base.supergroupid : base.userid;
	estrDestroy(&base.ziptext);
	return owner == atoi(value) ? 1 : 0;
}

int dbSyncContainerCreate(int list, int *id)
{
	valid &= list == CONTAINER_BASE;
	created++;
	*id = 1;
	return 1;
}

int dbSyncContainerRequest(int list, int id, int cmd, int no_process)
{
	valid &= list == CONTAINER_BASE && id == 1 &&
		cmd == CONTAINER_CMD_LOCK_AND_LOAD && no_process;
	return 1;
}

void dbAsyncContainerUpdate(int list, int id, int cmd, const char *data, int cb)
{
	valid &= list == CONTAINER_BASE && id == 1 &&
		cmd == CONTAINER_CMD_UNLOCK && !cb;
	free(stored);
	stored = strdup(data);
}

int dbSyncContainerRequestCustom(int list, int id, int cmd, NetPacketCallback *cb)
{
	valid &= list == CONTAINER_BASE && id == 1 &&
		cmd == CONTAINER_CMD_TEMPLOAD && cb == s_processBase;
	dbContainerUnpack(base_desc, stored, &s_requested_base);
	return 1;
}

int dbReadContainer(Packet *pak, ContainerInfo *ci, int list)
{
	// Packet transport is outside this persistence-contract test.
	return 0;
}

static void checkOwner(int sg, int user)
{
	char *text = NULL;
	for (int i = 0; i < 4096; i++)
		estrConcatCharString(&text, "Room \"HEXX \\ UTF-8 \xc3\xa9\"\n");
	created = 0;
	valid &= !strcmp(baseLoadText(sg, user), "");
	baseSaveText(sg, user, text);
	valid &= created == 1 && !strcmp(baseLoadText(sg, user), text);
	valid &= !strstr(stored, "\nData ") && strstr(stored, "ZipData ");
	baseSaveText(sg, user, "Room updated\n");
	valid &= created == 1 && !strcmp(baseLoadText(sg, user), "Room updated\n");
	baseSaveText(sg, user, "");
	valid &= created == 1 && !strcmp(baseLoadText(sg, user), "");
	estrDestroy(&text);
	free(stored);
	stored = NULL;
}

int main(void)
{
	char *template = dbContainerTemplate(base_desc);
	valid &= strstr(template, "ZipData \"binary(max)\"") != NULL;
	valid &= strstr(template, "\nData ") == NULL;
	free(template);
	checkOwner(41, 0);
	checkOwner(0, 42);
	puts(valid ? "Base persistence passed" : "Base persistence failed");
	return valid ? 0 : 1;
}
