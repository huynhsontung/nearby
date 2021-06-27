#ifndef STRINGPRINTF_H
#define STRINGPRINTF_H

#include <string>

void StringAppendV(std::string* dst, const char* format, va_list ap);

#endif