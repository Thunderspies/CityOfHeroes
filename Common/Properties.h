#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <utilitieslib/utils/structDefines.h>
#include <utilitieslib/utils/textparser_types.h>


#define PROPERTY_STRLEN        1024

typedef enum EditorPropertyType {
    PROPTYPE_NONE,
    PROPTYPE_CATEGORY,
    PROPTYPE_TEXTBOX,
    PROPTYPE_COMBOBOX,
    PROPTYPE_RADIOBUTTONS,
    PROPTYPE_SLIDER,
    PROPTYPE_INTEGERSLIDER,
    PROPTYPE_CHECKBOX,
    PROPTYPE_CHECKBOXLIST,
    PROPTYPE_BUTTON,
    PROPTYPE_PROPLIST,
    PROPTYPE_COMBOTEXTBOX,
} EditorPropertyType;

typedef struct PropertyDef PropertyDef;

typedef struct PropertyDef {
    // Values that are parsed in by the text parser
    char*                name;
    EditorPropertyType    type;
    char**                texts;
    char*                callback;
    int                    min;
    int                    max;
    bool                forceAutoSave;
    PropertyDef**        props;

    // Values that are not used by the text parser
    void*                data;
    int                    widgetId;
} PropertyDef;

typedef struct PropertyDefList
{
    PropertyDef ** list;
} PropertyDefList;

extern DefineIntList ParsePropertyTypeEnum[];
extern TokenizerParseInfo ParsePropertyDef[];
extern TokenizerParseInfo ParsePropertyDefList[];

void PropertyCopyData(const PropertyDef* src, PropertyDef* dest);
PropertyDef* PropertyClone(const PropertyDef* prop);
void PropertyDestroy(PropertyDef* prop);
PropertyDef** PropertyFindChangedProperties(PropertyDef* oldProp, PropertyDef* newProp);
PropertyDef* PropertyFindByName(char* propName, PropertyDef** properties);
PropertyDef** PropertyFindAllByType(EditorPropertyType propType, PropertyDef** properties);
char* PropertyValueStr(PropertyDef* property);
char* PropertyValueStrFromName(char* propName, PropertyDef** properties);

#endif