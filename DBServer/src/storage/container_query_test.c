#include "container/container_query.h"
#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "Failed at line %d: %s\n", __LINE__, #x); return 1; } } while (0)

int main(void)
{
    ContainerQuery query;
    ContainerMutation mutation;
    S64 id = 7;
    ContainerValue value = {CONTAINER_VALUE_INT, &id, sizeof(id)};
    int i;
    memset(&query, 0xff, sizeof(query));
    containerQueryInit(&query, 3, "Ents");
    CHECK(query.listId == 3 && !strcmp(query.table, "Ents"));
    CHECK(!query.fieldCount && !query.filterCount && !query.orderCount && !query.limit);
    for (i = 0; i < CONTAINER_QUERY_MAX_FIELDS; ++i)
        CHECK(containerQueryAddField(&query, "Name"));
    CHECK(!containerQueryAddField(&query, "Overflow"));
    CHECK(query.fieldCount == CONTAINER_QUERY_MAX_FIELDS);
    for (i = 0; i < CONTAINER_QUERY_MAX_FILTERS; ++i)
        CHECK(containerQueryAddFilter(&query, "ContainerId", CONTAINER_COMPARE_EQUAL, value));
    CHECK(!containerQueryAddFilter(&query, "Overflow", CONTAINER_COMPARE_EQUAL, value));
    CHECK(query.filters[0].value.data == &id && !query.filters[0].joinWithOr);
    for (i = 0; i < CONTAINER_QUERY_MAX_ORDER; ++i)
        CHECK(containerQueryAddOrder(&query, "ContainerId", true));
    CHECK(!containerQueryAddOrder(&query, "Overflow", false));
    CHECK(query.order[0].descending && query.orderCount == CONTAINER_QUERY_MAX_ORDER);
    memset(&mutation, 0xff, sizeof(mutation));
    containerMutationInit(&mutation, 3, "Ents");
    CHECK(mutation.listId == 3 && !strcmp(mutation.table, "Ents"));
    CHECK(!mutation.filterCount && !mutation.valueCount);
    return 0;
}
