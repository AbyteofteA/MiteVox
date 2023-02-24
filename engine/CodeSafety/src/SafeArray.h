#ifndef SAFEARRAY_H
#define SAFEARRAY_H

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

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
        inline SafeArray(const SafeArray<T>& otherArray);
        inline SafeArray(size_t elementCount);
        inline void deallocate();
        inline void clear();
        inline void reserve(size_t newElementCount);
        inline void resize(size_t newElementCount);
        inline void shrinkToFit();

        inline size_t getElementsCount() const;
        inline size_t getCapacity() const;
        inline size_t getSize();
        inline T* getElementsArray();
        inline T getElement(size_t index) const;
        inline T* getElementPointer(size_t index) const;
        inline T getLastElement() const;
        inline T* getLastElementPointer() const;
        inline void setElement(size_t index, T value);
        inline void setAllElements(T value);
        inline void insertElement(size_t index, T value); // TODO: 
        inline void appendElement(T value);
        inline void removeElement(size_t index); // TODO: 
        inline void removeElementAndSwapWithLast(size_t index);
        inline void removeLastElement();
        inline void fillWithZeros();

        inline void swap(size_t index1, size_t index2);

        inline void concatenate(SafeArray<T>* arrayToConcatenate);
        inline void concatenate(std::vector<T>* arrayToConcatenate);
        inline SafeArray<T>* getPart(size_t startIdex, size_t elementCount);
        inline void copyTo(SafeArray<T>* destination);

        inline std::string toASCII();
        inline void fromASCII(std::string arrayData);
        inline std::string toBase64(); // TODO: 
        inline void fromBase64(std::string arrayData); // TODO: 

        inline SafeArray<T>& operator=(const SafeArray<T>& otherArray);
        inline T& operator[](size_t index);

        inline T operator*(SafeArray<T>& otherArray);
        inline void operator*=(T multiplier);
        inline void operator/=(T divider);
        inline void operator+=(T value);
        inline void operator+=(SafeArray<T>& otherArray);
        inline void operator-=(T value);
        inline void operator-=(SafeArray<T>& otherArray);

    private:

        T* _elements = nullptr;
        size_t _elementsCount = 0;
        size_t _capacity = 0;
    };


    // IMPLEMENTATION BELOW //


    template <typename T>
    inline SafeArray<T>::SafeArray()
    {
        _elements = nullptr;
        _elementsCount = 0;
        _capacity = 0;
    }

    template <typename T>
    inline SafeArray<T>::SafeArray(const SafeArray<T>& otherArray) : SafeArray<T>::SafeArray()
    {
        *this = otherArray;
    }

    template <typename T>
    inline SafeArray<T>::SafeArray(size_t elementCount) : SafeArray<T>::SafeArray()
    {
        resize(elementCount);
    }

    template <typename T>
    inline void SafeArray<T>::deallocate()
    {
        delete[] _elements;
        _elements = nullptr;
        _elementsCount = 0;
        _capacity = 0;
    }

    template <typename T>
    inline void SafeArray<T>::clear()
    {
        _elementsCount = 0;
    }

    template <typename T>
    inline void SafeArray<T>::reserve(size_t newElementCount)
    {
        if (newElementCount == 0)
        {
            // TODO: log WARNING message.

            return;
        }

        if (_capacity >= newElementCount)
        {
            return;
        }

        T* tmpElements = new T[newElementCount];
        memcpy(tmpElements, _elements, _elementsCount * sizeof(T));
        delete[] _elements;
        _elements = tmpElements;
        _capacity = newElementCount;
    }

    template <typename T>
    inline void SafeArray<T>::resize(size_t newElementCount)
    {
        reserve(newElementCount);
        _elementsCount = newElementCount;
    }

    template <typename T>
    inline void SafeArray<T>::shrinkToFit()
    {
        if (_elementsCount == _capacity)
        {
            return;
        }

        T* tmpElements = new T[_elementsCount];
        memcpy(tmpElements, _elements, _elementsCount * sizeof(T));
        delete[] _elements;
        _elements = tmpElements;
        _capacity = _elementsCount;
    }

    template <typename T>
    inline size_t SafeArray<T>::getElementsCount() const
    {
        return _elementsCount;
    }

    template <typename T>
    inline size_t SafeArray<T>::getCapacity() const
    {
        return _capacity;
    }

    template <typename T>
    inline size_t SafeArray<T>::getSize()
    {
        return _elementsCount * sizeof(T);
    }

    template <typename T>
    inline T* SafeArray<T>::getElementsArray()
    {
        return _elements;
    }

    template <typename T>
    inline T SafeArray<T>::getElement(size_t index) const
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

            return T();
        }
        return _elements[index];
    }

    template <typename T>
    inline T* SafeArray<T>::getElementPointer(size_t index) const
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

            return nullptr;
        }
        return _elements + index;
    }

    template <typename T>
    inline T SafeArray<T>::getLastElement() const
    {
        if (_elementsCount == 0)
        {
            // TODO: log ERROR message.

            return T();
        }
        return _elements[_elementsCount - 1];
    }

    template <typename T>
    inline T* SafeArray<T>::getLastElementPointer() const
    {
        if (_elementsCount == 0)
        {
            // TODO: log ERROR message.

            return nullptr;
        }
        return getElementPointer(_elementsCount - 1);
    }

    template <typename T>
    inline void SafeArray<T>::setElement(size_t index, T value)
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

            return;
        }
        _elements[index] = value;
    }

    template <typename T>
    inline void SafeArray<T>::setAllElements(T value)
    {
        for (size_t index = 0; index < _elementsCount; ++index)
        {
            _elements[index] = value;
        }
    }

    template <typename T>
    inline void SafeArray<T>::insertElement(size_t index, T value)
    {
        if (index > _elementsCount)
        {
            // TODO: log ERROR message.

            return;
        }
        resize(_elementsCount + 1);
        // TODO:
    }

    template <typename T>
    inline void SafeArray<T>::appendElement(T value)
    {
        resize(_elementsCount + 1);
        _elements[_elementsCount - 1] = value;
    }

    template <typename T>
    inline void SafeArray<T>::removeElement(size_t index)
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

            return;
        }
        // TODO: 
    }

    template <typename T>
    inline void SafeArray<T>::removeElementAndSwapWithLast(size_t index)
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

            return;
        }
        _elements[index] = _elements[_elementsCount - 1];
        _elementsCount -= 1;
    }

    template <typename T>
    inline void SafeArray<T>::removeLastElement()
    {
        _elementsCount -= 1;
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
    inline void SafeArray<T>::swap(size_t index1, size_t index2)
    {
        if ((index1 >= _elementsCount) ||
            (index2 >= _elementsCount))
        {
            // TODO: log ERROR message.

            return;
        }
        T element1 = _elements[index1];
        T element2 = _elements[index2];
        _elements[index1] = element2;
        _elements[index2] = element1;
    }

    template <typename T>
    inline void SafeArray<T>::concatenate(SafeArray<T>* arrayToConcatenate)
    {
        size_t arrayToConcatenateElementCount = arrayToConcatenate->getElementsCount();
        if (arrayToConcatenateElementCount == 0)
        {
            // TODO: log WARNING message.

            return;
        }

        size_t oldElementCount = _elementsCount;
        resize(_elementsCount + arrayToConcatenateElementCount);

        for (size_t i = 0; i < arrayToConcatenateElementCount; ++i)
        {
            _elements[oldElementCount + i] = arrayToConcatenate->getElement(i);
        }
    }

    template <typename T>
    inline void SafeArray<T>::concatenate(std::vector<T>* arrayToConcatenate)
    {
        size_t arrayToConcatenateElementCount = arrayToConcatenate->size();
        if (arrayToConcatenateElementCount == 0)
        {
            // TODO: log WARNING message.

            return;
        }

        size_t oldElementCount = _elementsCount;
        resize(_elementsCount + arrayToConcatenateElementCount);

        for (size_t i = 0; i < arrayToConcatenateElementCount; ++i)
        {
            _elements[oldElementCount + i] = arrayToConcatenate->at(i);
        }
    }

    template <typename T>
    inline SafeArray<T>* SafeArray<T>::getPart(size_t startIndex, size_t elementCount)
    {
        if (this->_elementsCount < startIndex + elementCount)
        {
            // TODO: log WARNING message.

            size_t redundantElementsCount = (startIndex + elementCount) - this->_elementsCount;
            elementCount -= redundantElementsCount;
        }

        SafeArray<T>* arrayPart = new SafeArray<T>(elementCount);
        memcpy(arrayPart->getElementsArray(), this->_elements + startIndex, elementCount * sizeof(T));

        return arrayPart;
    }

    template <typename T>
    inline void SafeArray<T>::copyTo(SafeArray<T>* destination)
    {
        destination->resize(this->_elementsCount);
        memcpy(destination->getElementsArray(), this->_elements, this->_elementsCount * sizeof(T));
    }

    template <typename T>
    inline std::string SafeArray<T>::toASCII()
    {
        size_t arraySize = getSize();
        std::string result((char*)_elements, arraySize);
        return result;
    }

    template <typename T>
    inline void SafeArray<T>::fromASCII(std::string arrayData)
    {
        size_t arraySize = arrayData.size() / sizeof(T);
        resize(arraySize);
        memcpy(_elements, arrayData.data(), arraySize);
    }

    /*template <typename T>
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
    }*/

    /*template <typename T>
    inline void SafeArray<T>::fromBase64(std::string arrayData)
    {

    }*/

    template <typename T>
    inline SafeArray<T>& SafeArray<T>::operator=(const SafeArray<T>& otherArray)
    {
        size_t otherArrayElementsCount = otherArray.getElementsCount();
        if (_elementsCount != otherArrayElementsCount)
        {
            resize(otherArrayElementsCount);
        }
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] = otherArray.getElement(i);
        }
        return *this;
    }

    template <typename T>
    inline T& SafeArray<T>::operator[](size_t index)
    {
        if (index >= _elementsCount)
        {
            // TODO: log ERROR message.

        }
        return _elements[index];
    }

    template <typename T>
    inline T SafeArray<T>::operator*(SafeArray<T>& otherArray)
    {
        T resultScalar = (T)0;
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            resultScalar += _elements[i] * otherArray[i];
        }
        return resultScalar;
    }

    template <typename T>
    inline void SafeArray<T>::operator*=(T multiplier)
    {
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] *= multiplier;
        }
    }

    template <typename T>
    inline void SafeArray<T>::operator/=(T divider)
    {
        if (divider == (T)0)
        {
            return;
        }
        T multiplier = (T)1 / divider;
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] *= multiplier;
        }
    }

    template <typename T>
    inline void SafeArray<T>::operator+=(T value)
    {
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] += value;
        }
    }

    template <typename T>
    inline void SafeArray<T>::operator+=(SafeArray<T>& otherArray)
    {
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] += otherArray[i];
        }
    }

    template <typename T>
    inline void SafeArray<T>::operator-=(T value)
    {
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] -= value;
        }
    }

    template <typename T>
    inline void SafeArray<T>::operator-=(SafeArray<T>& otherArray)
    {
        for (size_t i = 0; i < _elementsCount; ++i)
        {
            _elements[i] -= otherArray[i];
        }
    }

    typedef SafeArray<uint8_t> SafeByteArray;
    typedef SafeArray<float> SafeFloatArray;
}

#endif