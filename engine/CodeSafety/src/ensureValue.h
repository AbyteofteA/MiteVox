
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

    /// <summary>
    /// Ensures that the value is at least minimumValue.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value"></param>
    /// <param name="minimumValue"></param>
    /// <returns></returns>
    template <typename T>
    T ensureMinimum(T value, T minimumValue)
    {
        if (value < minimumValue)
        {
            value = minimumValue;
        }
        return value;
    }

    /// <summary>
    /// Ensures that the value doesn't exceed maximumValue.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="value"></param>
    /// <param name="maximumValue"></param>
    /// <returns></returns>
    template <typename T>
    T ensureMaximum(T value, T maximumValue)
    {
        if (value > maximumValue)
        {
            value = maximumValue;
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