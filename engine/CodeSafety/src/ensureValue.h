
#ifndef ENSUREVALUE_H
#define ENSUREVALUE_H

namespace safety
{
    /// <summary>
    /// Ensures that the value is within the inclusive range of [rangeStart, rangeEnd].
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value"></param>
    /// <param name="rangeStart"></param>
    /// <param name="rangeEnd"></param>
    /// <returns></returns>
    template <typename T>
    T ensureRange(T value, T rangeStart, T rangeEnd)
    {
        if (value < rangeStart)
        {
            value = rangeStart;
        }
        else if (value > rangeEnd)
        {
            value = rangeEnd;
        }
        return value;
    }

    template <typename T>
    T ensureOddDown(T value)
    {
        if (value % 2 == 0)
        {
            value = value - 1;
        }
        return value;
    }

    template <typename T>
    T ensureOddUp(T value)
    {
        if (value % 2 == 0)
        {
            value = value + 1;
        }
        return value;
    }

    template <typename T>
    T ensureEvenDown(T value)
    {
        if (value % 2 == 1)
        {
            value = value - 1;
        }
        return value;
    }

    template <typename T>
    T ensureEvenUp(T value)
    {
        if (value % 2 == 1)
        {
            value = value + 1;
        }
        return value;
    }
}

#endif