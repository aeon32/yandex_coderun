//
// Created by user on 14.03.2024.
//
#include "python_exceptions.h"

PythonParserException::PythonParserException(const std::string & message, const std::string & place)
    :std::runtime_error(message), place(place)
{

}