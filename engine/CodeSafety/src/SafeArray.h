
#ifndef SAFEARRAY_H
#define SAFEARRAY_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>

namespace safety
{
    /// <summary>
    /// SafeArray is a safe but slow kind of array. 
    /// </summary>
    template <typename T>
    class SafeArray
    {
    public:

        inline SafeArray();
        inline SafeArray(int64_t elementCount);
        inline void deallocate();

        inline void resize(int64_t newElementCount);
        inline int64_t getElementsCount();
        inline int64_t getSize();
        inline T* getElementsArray();
        inline T getElement(int64_t index);
        inline void setElement(int64_t index, T value);
        inline void setAllElements(T value);
        inline void appendElement(T value);
        inline void fillWithZeros();
        inline void concatenate(SafeArray<T> arrayToConcatenate);

        inline std::string toASCII();
        inline void fromASCII(std::string arrayData);
        inline std::string toBase64();
        inline void fromBase64(std::string arrayData);

    private:

        T* _elements;
        int64_t _elementsCount;
    };


    // IMPLEMENTATION BELOW //


    template <typename T>
    inline SafeArray<T>::SafeArray()
    {
        _elements = nullptr;
        _elementsCount = 0;
    }

    template <typename T>
    inline SafeArray<T>::SafeArray(int64_t elementCount) : SafeArray<T>::SafeArray()
    {
        resize(elementCount);
    }

    template <typename T>
    inline void SafeArray<T>::deallocate()
    {
        delete[] _elements;
        _elements = nullptr;
        _elementsCount = 0;
    }

    template <typename T>
    inline void SafeArray<T>::resize(int64_t newElementCount)
    {
        if (newElementCount < 1)
        {
            // TODO: log WARNING message.

            return;
        }

        delete[] _elements;
        _elements = new T[newElementCount];
        _elementsCount = newElementCount;
    }

    template <typename T>
    inline int64_t SafeArray<T>::getElementsCount()
    {
        return _elementsCount;
    }

    template <typename T>
    inline int64_t SafeArray<T>::getSize()
    {
        return _elementsCount * sizeof(T);
    }

    template <typename T>
    inline T* SafeArray<T>::getElementsArray()
    {
        return _elements;
    }

    template <typename T>
    inline T SafeArray<T>::getElement(int64_t index)
    {
        if ((index < 0) || (index >= _elementsCount))
        {
            // TODO: log WARNING message.

            return T();
        }

        return _elements[index];
    }

    template <typename T>
    inline void SafeArray<T>::setElement(int64_t index, T value)
    {
        if ((index < 0) || (index >= _elementsCount))
        {
            // TODO: log WARNING message.

            return;
        }

        _elements[index] = value;
    }

    template <typename T>
    inline void SafeArray<T>::setAllElements(T value)
    {
        for (int64_t index = 0; index < _elementsCount; ++index)
        {
            _elements[index] = value;
        }
    }

    template <typename T>
    inline void SafeArray<T>::appendElement(T value)
    {
        _elementsCount += 1;
        delete[] _elements;
        _elements = new T[_elementsCount];
        _elements[_elementsCount - 1] = value;
    }

    template <typename T>
    inline void SafeArray<T>::fillWithZeros()
    {
        if ((_elements == nullptr) || (_elementsCount == 0))
        {
            // TODO: log WARNING message.

            return;
        }

        memset(_elements, 0, _elementsCount * sizeof(T));
    }

    template <typename T>
    inline void SafeArray<T>::concatenate(SafeArray<T> arrayToConcatenate)
    {
        int64_t arrayToConcatenateElementCount = arrayToConcatenate.getElementsCount();

        if (arrayToConcatenateElementCount == 0)
        {
            // TODO: log WARNING message.

            return;
        }

        _elements = (T*)realloc(_elements, (_elementsCount + arrayToConcatenateElementCount) * sizeof(T));
        memcpy(_elements + _elementsCount, arrayToConcatenate.getElementsArray(), arrayToConcatenateElementCount * sizeof(T));
        _elementsCount += arrayToConcatenateElementCount;
    }

    template <typename T>
    inline std::string SafeArray<T>::toASCII()
    {
        int64_t arraySize = getSize();
        std::string result((char*)_elements, arraySize);
        return result;
    }

    template <typename T>
    inline void SafeArray<T>::fromASCII(std::string arrayData)
    {
        int64_t arraySize = arrayData.size() / sizeof(T);
        resize(arraySize);
        memcpy(_elements, arrayData.data(), arraySize);
    }

    // TODO: 
    template <typename T>
    inline std::string SafeArray<T>::toBase64()
    {

        size_t base64Size = (_elementsCount + _elementsCount % 3) * 4 / 3;
        std::string base64result(base64Size, '\0');

        uint8_t remainingBits = 0;
        uint8_t remainingBitsCount = 0;
        for (size_t i = 0; i < base64Size; ++i)
        {
            char symbol = _elements[i] >> 2;
            base64result[i] = symbol;
        }

        return base64result;
    }

    template <typename T>
    inline void SafeArray<T>::fromBase64(std::string arrayData)
    {

    }

    typedef SafeArray<uint8_t> SafeByteArray;
    typedef SafeArray<float> SafeFloatArray;
}

#endif