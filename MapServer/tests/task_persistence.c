#include "container/dbcontainerpack.h"
#include "comm_backend.h"
#include "storyarc/storyarcprivate.h"
#include "storyarc/taskdef.h"
#include <stdio.h>
#include <string.h>

TaskHandle TaskGetHandle(const char *filename)
{
	return !strcmp(filename, "current_task.contact") ? 41 : 0;
}

const char *TaskFileName(TaskHandle handle)
{
	return handle == 41 ? "current_task.contact" : "";
}

#include "task_persistence.inc"

static StructDesc description = {
	sizeof(StoryTaskInfo), {AT_NOT_ARRAY, {{0}}}, task_line_desc
};

int main(void)
{
	StoryTaskInfo original = {0}, loaded = {0};
	original.sahandle.context = 41;
	original.sahandle.subhandle = 3;
	original.sahandle.compoundPos = 2;
	original.seed = 81234;
	original.state = TASK_ASSIGNED;
	original.haveClues[0] = 5;
	original.completeObjectives[0].set = 1;
	original.completeObjectives[0].compoundPos = 2;
	original.completeObjectives[0].num = 3;
	original.completeObjectives[0].success = 1;
	original.completeObjectives[0].missionobjective = 1;
	original.level = 10;
	original.timeout = 200123;
	original.assignedDbId = 42;
	original.assignedTime = 100123;
	original.missionMapId = 124;
	original.doorMapId = 125;
	original.doorPos[0] = 1.25f;
	original.doorPos[1] = -2.5f;
	original.doorPos[2] = 0.5f;
	original.curKillCount = 4;
	original.curKillCount2 = 6;
	original.nextLocation = 3;
	original.subtaskSuccess[0] = 5;
	original.teamCompleted = 1;
	original.completeSideObjectives = 7;
	original.difficulty.levelAdjust = -1;
	original.difficulty.teamSize = 4;
	original.difficulty.alwaysAV = 1;
	original.difficulty.dontReduceBoss = 1;
	original.timerType = -1;
	original.failOnTimeout = 1;
	original.timezero = 200000;

	char *template = dbContainerTemplate(&description);
	int valid = !strstr(template, "MysteryInvestigation") &&
		strstr(template, "ID \"int4\" attribute");
	free(template);
	for (int i = 0; i < 2; i++) {
		char *record = dbContainerPackage(&description, &original);
		memset(&loaded, 0, sizeof(loaded));
		dbContainerUnpack(&description, record, &loaded);
		valid &= !memcmp(&original, &loaded, sizeof(original));
		free(record);
		original = loaded;
	}
	puts(valid ? "Task persistence passed" : "Task persistence failed");
	return valid ? 0 : 1;
}
