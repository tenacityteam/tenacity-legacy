/**********************************************************************

  Tenacity

  WideStrings.h

**********************************************************************/

#ifndef WIDE_STRINGS_H
#define WIDE_STRINGS_H

#include <string>
#include <locale>
#include <vector>
#include <functional>

/* Allows you to widen a string
 
   For example:
      std::string s = "my test string";
      Widen<wchar_t> to_wstring;
      std::wstring w_s = to_wstring(s);
*/ 
template<class E, class T = std::char_traits<E>, class A = std::allocator<E>>
class Widen : public std::string, std::basic_string<E, T, A>{
    std::locale locale;
    const std::ctype<E>* c_type;

    // Don't allow copy or assignment
    inline Widen(const Widen&) {};
    inline Widen& operator= (const Widen&) {};


    public:
        inline Widen(const std::locale& given_locale = std::locale()) : locale(given_locale){
                c_type = &std::use_facet<std::ctype<E> >(given_locale);
        }

        std::basic_string<E, T, A> operator() (const std::string& str) const {
            typename std::basic_string<E, T, A>::size_type const string_length = str.length();
            const char* pointer_begin = str.c_str();
            std::vector<E> result(string_length);

            c_type->widen(pointer_begin, pointer_begin + string_length, &result[0]);
            return std::basic_string<E, T, A>(&result[0], string_length);
        }

};
#endif
