#include "cdohelpers.h"

ADLHelpers::ADLHelpers()
{
}


void ADLHelpers::stdString2ADLString(ADLString* target, const std::string& src)
{
    strncpy_s(target->body, ADL_STRING_MAX_LEN, src.c_str(),
          src.length());
    target->length = src.length();
}

ADLString ADLHelpers::stdString2ADLString(const std::string& src)
{
    ADLString result;
    stdString2ADLString(&result, src);
    return result;
}

std::string ADLHelpers::ADLString2Std(const ADLString* src)
{
    return std::string(src->body, src->length);
}

QString ADLHelpers::ADLString2QString(const ADLString* src)
{
    return QString::fromAscii(src->body, src->length);
}


bool ADLHelpers::stringEq(const ADLString* a, const std::string& b)
{
    return b == ADLHelpers::ADLString2Std(a);
}
