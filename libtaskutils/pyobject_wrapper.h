#ifndef PYOBJECT_WRAPPER_H
#define PYOBJECT_WRAPPER_H

#include <cstddef>
#include <iterator>

typedef struct _object PyObject;

class PyObject_wrapper
{
 protected:
    // Friend declarations.
    PyObject* mpObject;

 public:
    /// element_type
    /// This typedef is present for consistency with the C++ standard library
    /// auto_ptr template. It allows users to refer to the templated type via
    /// a typedef. This is sometimes useful to be able to do.
    ///
    /// Example usage:
    ///    PyObject_wrapper<IWidget> ip;
    ///    void DoSomething(PyObject_wrapper<IWidget>::element_type someType);
    ///
    typedef PyObject element_type;

    /// Default constructor. The member object is set to NULL.
    PyObject_wrapper()
        : mpObject(nullptr)
    {
        // Empty
    }

    /// Provides a constructor which takes ownership of a pointer.
    /// The incoming pointer is AddRefd.
    ///
    PyObject_wrapper(PyObject * p, bool bAddRef = true)
        : mpObject(p)
    {
        if (mpObject && bAddRef)
            add_ref(mpObject);// Intentionally do not prefix the call with eastl:: but instead allow namespace lookup to resolve the namespace.
    }

    /// PyObject_wrapper
    /// Construction from self type.
    PyObject_wrapper(const PyObject_wrapper& ip)
        : mpObject(ip.mpObject)
    {
        if (mpObject)
            add_ref(mpObject);
    }

    /// PyObject_wrapper
    /// move constructor
    PyObject_wrapper(PyObject_wrapper&& ip)
        : mpObject(nullptr)
    {
        swap(ip);
    }

    /// PyObject_wrapper
    /// Releases the owned pointer.
    ~PyObject_wrapper()
    {
        if (mpObject)
            release_ref(mpObject);
    }

    /// operator=
    /// Assignment to self type.
    PyObject_wrapper& operator=(const PyObject_wrapper& ip)
    {
        reset(ip.mpObject);
        return *this;
    }

    /// operator=
    /// Move assignment operator
    PyObject_wrapper& operator=(PyObject_wrapper&& ip)
    {
        swap(ip);
        return *this;
    }

    /// reset
    void reset(PyObject * pObject)
    {
        if (pObject != mpObject)
        {
            PyObject * const pTemp = mpObject;// Create temporary to prevent possible problems with re-entrancy.
            if (pObject)
                add_ref(pObject);
            mpObject = pObject;
            if (pTemp)
                release_ref(pTemp);
        }
    }


    /// get()
    /// Returns a pointer to the contained object.
    PyObject* get() const
    {
        return mpObject;
    }


    /// swap
    /// Exchanges the owned pointer beween two PyObject_wrapper objects.
    void swap(PyObject_wrapper& ip)
    {
        PyObject * const pTemp = mpObject;
        mpObject = ip.mpObject;
        ip.mpObject = pTemp;
    }

    /// attach
    /// Sets an PyObject_wrapper pointer without calling AddRef() on
    /// the pointed object. The PyObject_wrapper thus eventually only does a
    /// Release() on the object. This is useful for assuming a reference
    /// that someone else has handed you and making sure it is always
    /// released, even if you return in the middle of a function or an
    /// exception is thrown.
    ///
    void attach(PyObject* pObject)
    {
        PyObject* const pTemp = mpObject;
        mpObject = pObject;
        if (pTemp)
            release_ref(pTemp);
    }

    /// detach
    /// Surrenders the reference held by an PyObject_wrapper pointer --
    /// it returns the current reference and nulls the pointer. If the returned
    /// pointer is non-null it must be released. This is useful in functions
    /// that must return a reference while possibly being aborted by a return
    /// or thrown exception:
    PyObject* detach()
    {
        PyObject * const pTemp = mpObject;
        mpObject = nullptr;
        return pTemp;
    }

    /// operator!
    /// This returns the opposite of operator bool; it returns true if
    /// the owned pointer is null. Some compilers require this and some don't.
    bool operator!() const
    {
        return (mpObject == nullptr);
    }

    PyObject_wrapper str() const;

    const char * asString() const;
    long long int asInt() const;

    std::size_t len() const;


    /// iterator protocol
    struct PyObjectIterator_sentinel{};
    struct PyObjectIterator {
        //using iterator_category = std::input_iterator_tag;
        //using difference_type = std::ptrdiff_t;
        using value_type = PyObject_wrapper;
        using pointer = PyObject_wrapper *;
        using reference = PyObject_wrapper;
        using iterator_caterogory = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;

        PyObject_wrapper  operator*() const;

        PyObjectIterator & operator++();

        PyObjectIterator  operator++(int);

        PyObjectIterator(PyObject * iterator);
        ~PyObjectIterator();

        bool operator == (const PyObjectIterator_sentinel & ) const;

        PyObjectIterator(const PyObjectIterator & other) = delete;
        PyObjectIterator & operator = (const PyObjectIterator & other) = delete;

        PyObjectIterator(PyObjectIterator && other) ;
        PyObjectIterator & operator = (PyObjectIterator && other);

        PyObject * iterator;
        PyObject * value;
    };
    typedef PyObjectIterator iterator;

    PyObjectIterator begin();

    inline PyObjectIterator_sentinel end()
    {
        return PyObjectIterator_sentinel{};
    }


    static void add_ref(PyObject * obj);
    static void release_ref(PyObject * obj);

};// class PyObject_wrapper



#endif// Header include guard