#include "container_query.h"
#include <string.h>

void containerQueryInit(ContainerQuery *query, int listId, const char *table)
{
    memset(query, 0, sizeof(*query));
    query->listId = listId;
    query->table = table;
}

bool containerQueryAddField(ContainerQuery *query, const char *field)
{
    if (query->fieldCount == CONTAINER_QUERY_MAX_FIELDS)
        return false;
    query->fields[query->fieldCount++] = field;
    return true;
}

bool containerQueryAddFilter(ContainerQuery *query, const char *field, ContainerComparison comparison, ContainerValue value)
{
    ContainerFilter *filter;
    if (query->filterCount == CONTAINER_QUERY_MAX_FILTERS)
        return false;
    filter = &query->filters[query->filterCount++];
    memset(filter, 0, sizeof(*filter));
    filter->field = field;
    filter->comparison = comparison;
    filter->value = value;
    return true;
}

bool containerQueryAddOrder(ContainerQuery *query, const char *field, bool descending)
{
    ContainerOrder *order;
    if (query->orderCount == CONTAINER_QUERY_MAX_ORDER)
        return false;
    order = &query->order[query->orderCount++];
    order->field = field;
    order->descending = descending;
    return true;
}

void containerMutationInit(ContainerMutation *mutation, int listId, const char *table)
{
    memset(mutation, 0, sizeof(*mutation));
    mutation->listId = listId;
    mutation->table = table;
}
