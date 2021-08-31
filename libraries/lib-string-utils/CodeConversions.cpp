/*!********************************************************************

 Audacity: A Digital Audio Editor

 @file CodeConversions.cpp
 @brief Define functions to preform UTF-8 to std::wstring conversions.

 Dmitry Vedenko
 **********************************************************************/

#include "CodeConversions.h"

#include <locale>
#include <codecvt>

namespace audacity
{

std::string ToUTF8 (const std::wstring& wstr)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>> ().to_bytes (wstr);
}

std::string ToUTF8 (const wchar_t* wstr)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>> ().to_bytes (wstr);
}

std::string ToUTF8 (const wxString& wstr)
{
#if wxCHECK_VERSION(3, 1, 1)
    return wstr.ToStdString (wxGet_wxConvUTF8 ());
#elif !wxUSE_UNICODE
    return std::string (wstr.c_str (), wstr.length ());
#else
    wxScopedCharBuffer buf (wstr.mb_str (wxGet_wxConvUTF8 ()));
    return std::string (buf.data (), buf.length ());
#endif
}

std::wstring ToWString (const std::string& str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>> ().from_bytes (str);
}

std::wstring ToWString (const char* str)
{
    return std::wstring_convert<std::codecvt_utf8<wchar_t>> ().from_bytes (str);
}

std::wstring ToWString (const wxString& str)
{
    return str.ToStdWstring ();
}

wxString ToWXString (const std::string& str)
{
#if wxCHECK_VERSION(3, 1, 1)
    return wxString::FromUTF8 (str);
#else
    return wxString::FromUTF8 (str.c_str());
#endif
}

wxString ToWXString (const std::wstring& str)
{
    return wxString (str);
}

}
