//
// Created by user on 14.03.2024.
//
#ifndef _PYTHON_UTILS_H_
#define _PYTHON_UTILS_H_

#include <memory>
#include <exception>
#include <stdexcept>
#include <string>

#include "pyobject_wrapper.h"

typedef struct _object PyObject;
struct PyObject_deleter{ void operator() (PyObject * obj) const;};

using PyObject_unique_ptr = std::unique_ptr<PyObject, PyObject_deleter >;


/**\PythonInterp
 * Python interp context
 *
**/
class PythonInterp
{
 private:
    struct PythonInterpPrivate;
    struct PythonInterpPrivate_deleter { void operator()(PythonInterpPrivate * pimpl) const; };
    std::unique_ptr<PythonInterpPrivate, PythonInterpPrivate_deleter > m_impl;

 public:

    PythonInterp() noexcept;

    ~PythonInterp();

    PyObject_wrapper execString(const char * str);

    void simpleExec(const char* stringToExec);

    void pyExceptionalThrower(const char* errorMessagePrefix, bool logMessage);

    PythonInterp(const PythonInterp&) = delete;
    PythonInterp& operator=(const PythonInterp&) = delete;

    PythonInterp (PythonInterp && other):m_impl(std::move(other.m_impl))
    {
    }

    PythonInterp & operator = (PythonInterp && other)
    {
        m_impl = std::move(other.m_impl) ;
        return *this;
    }
};

#endif//_PYTHON_UTILS_H_
