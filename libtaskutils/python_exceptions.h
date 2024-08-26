//
// Created by user on 14.03.2024.
//

#ifndef _PYTHON_EXCEPTIONS_H_
#define _PYTHON_EXCEPTIONS_H_

#include <stdexcept>

struct PythonParserException:public std::runtime_error
{
    PythonParserException(const std::string & message, const std::string & place);
    std::string place;
};

#endif//_PYTHON_EXCEPTIONS_H_
