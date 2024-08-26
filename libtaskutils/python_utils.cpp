#include "python_utils.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <optional>
#include <sstream>

#include <Python.h>
#include "python_exceptions.h"

void PyObject_deleter::operator() (PyObject * obj) const
{
   Py_XDECREF(obj);
}


struct PythonInitializer
{
    PythonInitializer() {
        if (!Py_IsInitialized())
        {
            Py_InitializeEx(0);
        };
    };

    ~PythonInitializer()
    {
        if (Py_IsInitialized()) {
            Py_Finalize();
        };
    };
};

std::unique_ptr<PythonInitializer> pythonInitializer;

struct PythonInterp::PythonInterpPrivate
{
    static int nModule;

    bool tracebackModuleLoaded;

    PyObject_wrapper globalDict;
    PyObject_wrapper mainModule;
    PyObject_wrapper traceBackModule;
    PyObject_wrapper formatTraceBack;
    PyObject_wrapper formatException;


    PythonInterpPrivate() noexcept
    {
        if (!pythonInitializer.get())
        {
            pythonInitializer.reset(new PythonInitializer());
        };

        std::ostringstream aux;
        aux<<"myzxc"<<nModule++;
        tracebackModuleLoaded = false;

        PyObject_wrapper fromList(PyList_New(0));

        mainModule = PyObject_wrapper(PyImport_AddModule(aux.str().c_str()), false);
        globalDict = PyObject_wrapper(PyModule_GetDict(mainModule.get()), true);

        traceBackModule = PyObject_wrapper(PyImport_ImportModuleEx("traceback",globalDict.get(), globalDict.get(), fromList.get()), false);

        PyObject * traceBackModuleDict =PyModule_GetDict(traceBackModule.get());
        formatTraceBack = PyObject_wrapper(PyDict_GetItemString(traceBackModuleDict, "format_tb"), true);
        formatException = PyObject_wrapper(PyDict_GetItemString(traceBackModuleDict, "format_exception"), true);


    };

    ~PythonInterpPrivate() {

        //Py_DECREF(myModule);
        //Py_XDECREF(mainModule);

    };

    std::string pyUnicodeToString(PyObject *object)
    {
        std::string result;
        if (PyUnicode_Check(object))
        {
            PyObject *bytes = PyUnicode_AsEncodedString(object, "UTF-8", "strict"); // Owned reference
            if (bytes != nullptr)
            {
                char *utf8String = PyBytes_AS_STRING(bytes); // Borrowed pointer
                result = utf8String;
                Py_DECREF(bytes);
            }
        }
        return result;
    }

    /**
     *
     *
    **/
    void pyPrintError()
    {
        if (PyErr_Occurred())
        {
            PyObject * excType = 0;
            PyObject * pValue =0 ;
            PyObject * pTraceBack =0;
            PyErr_Fetch(&excType, &pValue, &pTraceBack);
            PyErr_Clear();

            PyObject * errorString = PyObject_Str(pValue);
            std::cerr<<pyUnicodeToString(errorString)<<std::endl;

            Py_XDECREF(errorString);
            Py_XDECREF(excType);
            Py_XDECREF(pValue);
            Py_XDECREF(pTraceBack);
        };

    };

    /**\func PySimpleError
     *
    **/
    std::optional<std::string> pySimpleError()
    {
        if (PyErr_Occurred()) {

            PyObject * excType = 0;
            PyObject * pValue =0 ;
            PyObject * pTraceBack =0;
            PyErr_Fetch(&excType, &pValue, &pTraceBack);
            std::string error;

            PyErr_Clear();
            if (pTraceBack)
            {
                PyObject * formatedException = PyObject_CallFunctionObjArgs(formatException.get(),excType,pValue,pTraceBack,NULL);
                if (PyErr_Occurred())
                {
                    PyErr_Clear();
                    error += PyObject_wrapper(pValue).str().asString();

                } else
                {
                    PyObject * iterator = PyObject_GetIter(formatedException);
                    while (PyObject * item = PyIter_Next(iterator) )
                    {
                        error += PyObject_wrapper(item, false).str().asString();
                    };
                    Py_DECREF(formatedException);
                };
            } else
            {
                if (PyErr_GivenExceptionMatches(excType, PyExc_SyntaxError))
                {
                    error = std::string("Syntax error in file ");
                    /*
                    PyTuple_GetItem(pValue,)
                    tuple aux =  boost::python::tuple(handle<> (pValue));

                    error += boost::python::extract<std::string>(str(aux[1][0]));
                    error += " at line ";
                    error +=  boost::python::extract<std::string>(str(aux[1][1]));
                    error += " near ";
                    error +=  boost::python::extract<std::string>(str(aux[1][3]));
                     */

                    //error += boost::python::extract<std::string>(aux[1][2]);

                    //error += "line
                } else
                {
                    error += PyObject_wrapper(excType).str().asString();
                    error +=": ";
                    error += PyObject_wrapper(pValue).str().asString();
                };
            };

            Py_XDECREF(excType);
            Py_XDECREF(pValue);
            Py_XDECREF(pTraceBack);


            return error;
        };
        return std::nullopt;
    };



    /**\func PyExceptionalThrower
     * Аналог PySimpleError, но результат возвращается исключением
     *
    **/
    /*
    void PyExceptionalThrower(const std::string & prefix, bool logMessage) {
        std::string res;
        if (PySimpleError(res)) {
            std::string output = prefix+res;
            EOM_LOG_ERROR<<output<<eom::endl;
            throw EPythonParserException(output,FILE_AND_LINE);

        };
    */
    };

    int PythonInterp::PythonInterpPrivate::nModule = 0;

    void PythonInterp::PythonInterpPrivate_deleter::operator()(PythonInterpPrivate * pimpl) const
    {
        delete pimpl;
    }


    PythonInterp::PythonInterp() noexcept
        :m_impl(new PythonInterpPrivate())
    {


    };

    PythonInterp::~PythonInterp()
    {

    };

    /*
    void PythonInterp::simpleExec(const char * stringToExec) {
        pyobject_unique_ptr  parseStringResult;

        parseStringResult = PyRun_String(stringToExec, Py_file_input, m_impl->globalDict.get(), m_impl->globalDict.get());

        impl->PyExceptionalThrower(std::string("Error processing string \n"),true);

    };
   */

    PyObject_wrapper PythonInterp::execString(const char * strToExec)
    {
        PyObject_wrapper res;
        res.attach(PyRun_String(strToExec, Py_eval_input, m_impl->globalDict.get(), m_impl->globalDict.get()));
        if (std::optional<std::string> err = m_impl->pySimpleError())
        {
            throw PythonParserException(err.value(), std::string());
        }
        return res;
    }



