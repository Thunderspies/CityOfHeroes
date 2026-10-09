#include "container/dbcontainerpack.h"
#include "entity/gametypes.h"
#include <utilitieslib/components/EString.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Payload {
	const char *attribute;
	char *binary;
	char *ascii;
	char *utf8;
} Payload;

static LineDesc fields[] = {
	{{PACKTYPE_ATTR, MAX_ATTRIBNAME_LEN, "Attribute",
		OFFSET(Payload, attribute), INOUT(0, 0),
		LINEDESCFLAG_INDEXEDCOLUMN}},
	{{PACKTYPE_LARGE_ESTRING_BINARY, 0, "Binary", OFFSET(Payload, binary)}},
	{{PACKTYPE_LARGE_ESTRING_ASCII, 0, "Ascii", OFFSET(Payload, ascii)}},
	{{PACKTYPE_LARGE_ESTRING_UTF8, 0, "Utf8", OFFSET(Payload, utf8)}},
	{0}
};

static StructDesc description = {
	sizeof(Payload), {AT_NOT_ARRAY, {{0}}}, fields
};

static int equalPayload(Payload *a, Payload *b)
{
	return estrLength(&a->binary) == estrLength(&b->binary) &&
		!strcmp(a->attribute, b->attribute) &&
		!memcmp(a->binary, b->binary, estrLength(&a->binary)) &&
		!strcmp(a->ascii, b->ascii) && !strcmp(a->utf8, b->utf8);
}

int main(void)
{
	Payload original = {0}, loaded = {0};
	original.attribute = "current_attribute";
	unsigned char bytes[256];
	for (int i = 0; i < sizeof(bytes); i++)
		bytes[i] = (unsigned char)i;
	estrConcatFixedWidth(&original.binary, (const char *)bytes, sizeof(bytes));
	estrPrintCharString(&original.ascii, "HEXX is ordinary text: \"quoted\"\\\n");
	estrPrintCharString(&original.utf8, "UTF-8: \xc3\xa9 \xe6\x97\xa5");

	char *template = dbContainerTemplate(&description);
	int valid = strstr(template, "Attribute \"int4\" attribute indexed") &&
		strstr(template, "\"binary(max)\"") &&
		strstr(template, "\"ansistring(max)\"") &&
		strstr(template, "\"unicodestring(max)\"");
	free(template);

	char *record = dbContainerPackage(&description, &original);
	dbContainerUnpack(&description, record, &loaded);
	valid = valid && equalPayload(&original, &loaded);
	free(record);

	// A second save/load must retain literal HEXX text and every binary byte.
	record = dbContainerPackage(&description, &loaded);
	estrDestroy(&loaded.binary);
	estrDestroy(&loaded.ascii);
	estrDestroy(&loaded.utf8);
	dbContainerUnpack(&description, record, &loaded);
	valid = valid && equalPayload(&original, &loaded);
	free(record);
	estrDestroy(&original.binary);
	estrDestroy(&original.ascii);
	estrDestroy(&original.utf8);
	estrDestroy(&loaded.binary);
	estrDestroy(&loaded.ascii);
	estrDestroy(&loaded.utf8);
	puts(valid ? "Container packing passed" : "Container packing failed");
	return valid ? 0 : 1;
}
