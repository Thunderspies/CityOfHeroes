#include <utilitieslib/stdtypes.h>
#include "Properties.h"
#include <utilitieslib/utils/textparser.h>
#include <utilitieslib/components/earray.h>
#include <utilitieslib/assert/assert.h>

DefineIntList ParsePropertyTypeEnum[] =
{
    DEFINE_INT
    {    "Category",            PROPTYPE_CATEGORY        },
    {    "TextBox",            PROPTYPE_TEXTBOX        },
    {    "Slider",            PROPTYPE_SLIDER            },
    {    "IntegerSlider",    PROPTYPE_INTEGERSLIDER    },
    {    "RadioButtons",        PROPTYPE_RADIOBUTTONS    },
    {    "ComboBox",            PROPTYPE_COMBOBOX        },
    {    "CheckBox",            PROPTYPE_CHECKBOX        },
    {    "CheckBoxList",        PROPTYPE_CHECKBOXLIST    },
    {    "Button",            PROPTYPE_BUTTON            },
    {    "ComboTextBox",        PROPTYPE_COMBOTEXTBOX    },
    {    "",                    0                        },
    DEFINE_END
};


TokenizerParseInfo ParsePropertyDef[] =
{
    { "{",                TOK_START,                                            0    },
    { "Name",            TOK_STRING(PropertyDef, name, 0),                                        },
    { "Type",            TOK_INT(PropertyDef, type, 0),    ParsePropertyTypeEnum    },
    { "Strings",        TOK_STRINGARRAY(PropertyDef, texts),                                        },
    { "Callback",        TOK_STRING(PropertyDef, callback,0),                                    },
    { "ForceAutoSave",    TOK_BOOLFLAG(PropertyDef, forceAutoSave,0),                                },
    { "Min",            TOK_INT(PropertyDef, min,0)                            },
    { "Max",            TOK_INT(PropertyDef, max,0)                            },
    { "Properties",        TOK_STRUCT(PropertyDef, props,    ParsePropertyDef) },
    { "}",                TOK_END,                                                    0                            },
    { "", 0, 0 }
};

TokenizerParseInfo ParsePropertyDefList[] =
{
    { "Properties",        TOK_STRUCT(PropertyDefList, list, ParsePropertyDef) },
    { "", 0, 0 }
};

// Copy the data, differs based on type
void PropertyCopyData(const PropertyDef* src, PropertyDef* dest)
{
    if (src != dest)
    {
        int i, n = eaSize(&src->props);
        if (src->type == PROPTYPE_TEXTBOX || src->type == PROPTYPE_COMBOTEXTBOX)
        {
            // If the parser made a shallow copy, we need to overwrite it
            if (!dest->data || src->data == dest->data)
                dest->data = malloc(PROPERTY_STRLEN);
            strcpy((char*)dest->data, (char*)src->data);
        }
        else
        {
            dest->data = src->data;
        }
        for (i = 0; i < n; i++)
            PropertyCopyData(src->props[i], dest->props[i]);
    }
}

PropertyDef* PropertyClone(const PropertyDef* prop)
{
    int i, n = eaSize(&prop->props);
    PropertyDef* newProp = ParserAllocStruct(sizeof(PropertyDef));
    ParserCopyStruct(ParsePropertyDef, (void*)prop, sizeof(PropertyDef), newProp);

    // Now copy the data from the property and all the sub properties
    PropertyCopyData(prop, newProp);
    for (i = 0; i < n; i++)
        PropertyCopyData(prop->props[i], newProp->props[i]);

    return newProp;
}

void PropertyDestroy(PropertyDef* prop)
{
    int i, n = eaSize(&prop->props);

    // Only need to free the strings
    if (prop->type == PROPTYPE_TEXTBOX || prop->type == PROPTYPE_COMBOTEXTBOX)
        free((char*)prop->data);

    // Now free all the subproperties of the property
    for (i = 0; i < n; i++)
        PropertyDestroy(prop->props[i]);
}

// Compares two properties and if they have different data, adds the old prop to the change list
static void propCompareData(PropertyDef*** changeList, PropertyDef* oldProp, PropertyDef* newProp)
{
    int i, n = eaSize(&oldProp->props);
    int differ = 0;

    // We are looking for the change between two widgets, they must have the same structure
    assert((oldProp->type == newProp->type) && (eaSize(&oldProp->props) == eaSize(&newProp->props)));

    // Compare the data differently for different types
    if (oldProp->type == PROPTYPE_TEXTBOX || oldProp->type == PROPTYPE_COMBOTEXTBOX)
        differ = strcmp((char*)oldProp->data, (char*)newProp->data);
    else
        differ = (oldProp->data != newProp->data);

    // If it differs, we want to save the old one so we can keep track of the changes
    if (differ)
        eaPush(changeList, oldProp);

    // Check for subproperties the same way
    for (i = 0; i < n; i++)
        propCompareData(changeList, oldProp->props[i], newProp->props[i]);
}

// Looks through all the subproperties and returns a list containing which properties changed
PropertyDef** PropertyFindChangedProperties(PropertyDef* oldProp, PropertyDef* newProp)
{
    static PropertyDef** changeList = NULL;
    int i, n = eaSize(&oldProp->props);

    // We are looking for the change between two widgets, they must have the same structure
    assert(eaSize(&oldProp->props) == eaSize(&newProp->props));

    // Now check all the sub properties, who will add their results to the array
    eaSetSize(&changeList, 0);
    for (i = 0; i < n; i++)
        propCompareData(&changeList, oldProp->props[i], newProp->props[i]);

    return changeList;
}

// Checks for propType and adds to match list on a match
static void propCompareType(PropertyDef*** matchList, EditorPropertyType propType, PropertyDef* property)
{
    int i, n = eaSize(&property->props);

    if (property->type == propType)
        eaPush(matchList, property);

    for (i = 0; i < n; i++)
        propCompareType(matchList, propType, property->props[i]);
}


// Returns a static EArray containing all properties of the given type
PropertyDef** PropertyFindAllByType(EditorPropertyType propType, PropertyDef** properties)
{
    static PropertyDef** matchList = NULL;
    int i, n = eaSize(&properties);
    
    eaSetSize(&matchList, 0);
    for (i = 0; i < n; i++)
        propCompareType(&matchList, propType, properties[i]);
    return matchList;
}

// Finds a PropertyDef of a given name in the PropertyDef list
PropertyDef* PropertyFindByName(char* propName, PropertyDef** properties)
{
    int i, n = eaSize(&properties);
    PropertyDef* property = NULL;
    for (i = 0; i < n; i++)
    {
        // We found the property we were looking for, return the value
        if (properties[i]->name && !stricmp(properties[i]->name, propName))
            return properties[i];

        // Check my children, if we found it, return it, otherwise continue
        if (eaSize(&properties[i]->props))
            property = PropertyFindByName(propName, properties[i]->props);
        if (property)
            return property;
    }
    return NULL;
}

// Returns a string that contains the property's value
char* PropertyValueStr(PropertyDef* property)
{
    static char propText[PROPERTY_STRLEN];

    switch (property->type)
    {
        xcase PROPTYPE_TEXTBOX:
        case PROPTYPE_COMBOTEXTBOX:
            return property->data;
        xcase PROPTYPE_COMBOBOX:
            if (!property->texts || ((intptr_t)property->data < 0) || ((intptr_t)property->data >= eaSize(&property->texts)))
                return NULL;
            return property->texts[(intptr_t)property->data];
        xcase PROPTYPE_RADIOBUTTONS:
            if (!property->texts || ((intptr_t)property->data < 0) || ((intptr_t)property->data >= eaSize(&property->texts)))
                return NULL;
            return property->texts[(intptr_t)property->data];
        xcase PROPTYPE_PROPLIST:
            if (!property->props || ((intptr_t)property->data < 0) || ((intptr_t)property->data >= eaSize(&property->props)))
                return NULL;
            return property->props[(intptr_t)property->data]->name;
        xcase PROPTYPE_INTEGERSLIDER:
            sprintf(propText, "%i", (U32)(*((F32*)&property->data)));
            return propText;
        xcase PROPTYPE_CHECKBOX:
        case PROPTYPE_CHECKBOXLIST:
            sprintf(propText, "%i", (intptr_t)property->data);
            return propText;
        xcase PROPTYPE_SLIDER:
            sprintf(propText, "%f", *((F32*)&property->data));
            return propText;
        xdefault:
            return NULL;
    }
}

// Searchs through an earray of properties to find a specified property name
char* PropertyValueStrFromName(char* propName, PropertyDef** properties)
{
    PropertyDef* property = PropertyFindByName(propName, properties);
    if (property)
        return PropertyValueStr(property);
    return NULL;
}
