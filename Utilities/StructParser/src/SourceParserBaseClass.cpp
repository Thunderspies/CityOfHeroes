#include "pch.h"
#include "sourceparserbaseclass.h"


SourceParserBaseClass::SourceParserBaseClass() : m_pParent(nullptr), m_iIndexInParent(0)
{}

SourceParserBaseClass::~SourceParserBaseClass()
{}

char *SourceParserBaseClass::GetAutoGenCFileName(void)
{
    return NULL;
}
    
char *SourceParserBaseClass::GetAutoGenCPPFileName(void)
{
    return NULL;
}
