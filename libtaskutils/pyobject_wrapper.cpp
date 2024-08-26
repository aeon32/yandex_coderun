//
// Created by user on 14.03.2024.
//
#include "pyobject_wrapper.h"

#include <Python.h>

#include "python_exceptions.h"

void PyObject_wrapper::add_ref(PyObject * obj)
{
    Py_INCREF(obj);
}

void PyObject_wrapper::release_ref(PyObject * obj)
{
    Py_DECREF(obj);
}

PyObject_wrapper PyObject_wrapper::str() const
{
    PyObject_wrapper str(mpObject);            //borrowed reference

    if (!PyUnicode_Check(str.get()))
    {
        // try cast to string
        str.attach(PyObject_Str(mpObject)); //owned reference
    }
    PyObject_wrapper value;
    value.attach(PyUnicode_AsUTF8String(str.get())); //owned reference

    return value;
}

const char * PyObject_wrapper::asString() const
{
    return PyBytes_AsString(mpObject);
}

long long int PyObject_wrapper::asInt() const
{
    assert(PyLong_Check(mpObject));
    return PyLong_AsLongLong(mpObject);
}

std::size_t PyObject_wrapper::len() const
{
    assert(PySequence_Check(mpObject));
    return PySequence_Length(mpObject);
}


PyObject_wrapper  PyObject_wrapper::PyObjectIterator::operator*() const
{
    return PyObject_wrapper(value, true);
}


PyObject_wrapper::PyObjectIterator & PyObject_wrapper::PyObjectIterator::operator++()
{
    PyObject * newValue = PyIter_Next(iterator);
    Py_XDECREF(value);
    value = newValue;
    return *this;
}

PyObject_wrapper::PyObjectIterator PyObject_wrapper::PyObjectIterator::operator++(int)
{
    assert(false); //not implemented
    return PyObject_wrapper::PyObjectIterator(nullptr);

}

PyObject_wrapper::PyObjectIterator::~PyObjectIterator()
{
    Py_XDECREF(iterator);
    Py_XDECREF(value);

}


bool PyObject_wrapper::PyObjectIterator::operator == (const PyObjectIterator_sentinel & ) const
{
    return value == nullptr;
}

PyObject_wrapper::PyObjectIterator PyObject_wrapper::begin()
{
    PyObject *iterator = PyObject_GetIter(mpObject);
    assert(iterator != nullptr);
    return PyObjectIterator(iterator);
}

PyObject_wrapper::PyObjectIterator::PyObjectIterator(PyObject * iterator)
                 :iterator(iterator), value(PyIter_Next(iterator))

{

}

PyObject_wrapper::PyObjectIterator::PyObjectIterator(PyObject_wrapper::PyObjectIterator && other)
   :iterator(other.iterator), value(other.value)
{
   other.iterator = nullptr;
   other.value = nullptr;
}

PyObject_wrapper::PyObjectIterator & PyObject_wrapper::PyObjectIterator::operator = (PyObjectIterator && other)
{
    if (iterator != other.iterator)
    {
        iterator = other.iterator;
        value = other.value;
        other.iterator = nullptr;
        other.value = nullptr;

    }
    return *this;
}
