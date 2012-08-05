#ifndef CDOHELPERS_H
#define CDOHELPERS_H

#include <cloudeo_sdk.h>

#include <string>
#include <QString>
class CDOHelpers
{
private:
    CDOHelpers();

public:

    static void stdString2CdoString(CDOString* target, const std::string& src);
    static CDOString stdString2CdoString(const std::string& src);
    static std::string cdoString2Std(const CDOString* src);
    static QString cdoString2QString(const CDOString* src);

    static bool stringEq(const CDOString* a, const std::string& b);
};

#endif // CDOHELPERS_H
