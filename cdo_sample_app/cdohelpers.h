#ifndef ADLHELPERS_H
#define ADLHELPERS_H

#include <addlive_sdk.h>

#include <string>
#include <QString>
class ADLHelpers
{
private:
    ADLHelpers();

public:

    static void stdString2ADLString(ADLString* target, const std::string& src);
    static ADLString stdString2ADLString(const std::string& src);
    static std::string ADLString2Std(const ADLString* src);
    static QString ADLString2QString(const ADLString* src);

    static bool stringEq(const ADLString* a, const std::string& b);
};

#endif // ADLHELPERS_H
