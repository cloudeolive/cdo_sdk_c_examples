#include "cdohelpers.h"

CDOHelpers::CDOHelpers()
{
}


void CDOHelpers::stdString2CdoString(CDOString* target, const std::string& src)
{
    strncpy_s(target->body, CDO_STRING_MAX_LEN, src.c_str(),
          src.length());
    target->length = src.length();
}

CDOString CDOHelpers::stdString2CdoString(const std::string& src)
{
    CDOString result;
    stdString2CdoString(&result, src);
    return result;
}
