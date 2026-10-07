#ifndef CONTAINER_QUERY_H
#define CONTAINER_QUERY_H

#include <utilitieslib/stdtypes.h>

C_DECLARATIONS_BEGIN

#define CONTAINER_QUERY_MAX_FIELDS 32
#define CONTAINER_QUERY_MAX_FILTERS 32
#define CONTAINER_QUERY_MAX_ORDER 8

typedef enum ContainerValueType { CONTAINER_VALUE_NULL, CONTAINER_VALUE_INT, CONTAINER_VALUE_FLOAT, CONTAINER_VALUE_TEXT, CONTAINER_VALUE_DATETIME, CONTAINER_VALUE_BLOB } ContainerValueType;
typedef enum ContainerComparison { CONTAINER_COMPARE_EQUAL, CONTAINER_COMPARE_NOT_EQUAL, CONTAINER_COMPARE_LESS, CONTAINER_COMPARE_LESS_EQUAL, CONTAINER_COMPARE_GREATER, CONTAINER_COMPARE_GREATER_EQUAL, CONTAINER_COMPARE_LIKE, CONTAINER_COMPARE_IS_NULL, CONTAINER_COMPARE_IS_NOT_NULL } ContainerComparison;
typedef enum ContainerJoinType { CONTAINER_JOIN_NONE, CONTAINER_JOIN_INNER, CONTAINER_JOIN_LEFT } ContainerJoinType;

// Borrowed input data: integers are S64, floats are double, text is UTF-8.
// Sizes exclude the terminator. BLOB data may contain embedded NUL bytes.
typedef struct ContainerValue { ContainerValueType type; const void *data; size_t size; } ContainerValue;
typedef struct ContainerFilter { const char *table; const char *field; ContainerComparison comparison; ContainerValue value; bool joinWithOr; } ContainerFilter;
typedef struct ContainerOrder { const char *field; bool descending; } ContainerOrder;
typedef struct ContainerJoin { ContainerJoinType type; const char *table; const char *leftField; const char *rightField; } ContainerJoin;

typedef struct ContainerQuery
{
    int listId;
    const char *table;
    const char *fields[CONTAINER_QUERY_MAX_FIELDS];
    int fieldCount;
    ContainerFilter filters[CONTAINER_QUERY_MAX_FILTERS];
    int filterCount;
    ContainerOrder order[CONTAINER_QUERY_MAX_ORDER];
    int orderCount;
    ContainerJoin join;
    int limit;
    bool distinct;
} ContainerQuery;

typedef struct ContainerMutation
{
    int listId;
    const char *table;
    ContainerFilter filters[CONTAINER_QUERY_MAX_FILTERS];
    int filterCount;
    const char *fields[CONTAINER_QUERY_MAX_FIELDS];
    ContainerValue values[CONTAINER_QUERY_MAX_FIELDS];
    int valueCount;
} ContainerMutation;

// Initializers borrow table names; add helpers borrow fields/filter data.
// Add helpers return false at capacity without changing the description.
void containerQueryInit(ContainerQuery *query, int listId, const char *table);
bool containerQueryAddField(ContainerQuery *query, const char *field);
bool containerQueryAddFilter(ContainerQuery *query, const char *field, ContainerComparison comparison, ContainerValue value);
bool containerQueryAddOrder(ContainerQuery *query, const char *field, bool descending);
void containerMutationInit(ContainerMutation *mutation, int listId, const char *table);

C_DECLARATIONS_END
#endif
