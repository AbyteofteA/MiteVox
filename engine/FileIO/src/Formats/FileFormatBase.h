#ifndef FILEFORMATBASE_H
#define FILEFORMATBASE_H

#include "engine/FileIO/src/FileStatus.h"
#include "engine/FileIO/src/SerializationStatus.h"
#include "engine/CodeSafety/src/SafeArray.h"

#include <string>

namespace fileio
{
    class FileFormatBase
    {
    public:

        FileStatus saveToFile(std::string filename);
        FileStatus readFromFile(std::string filename);

        virtual SerializationStatus serialize(safety::SafeByteArray* fileData);
        virtual DeserializationStatus deserialize(safety::SafeByteArray* fileData);

    protected:

        std::string _path;
    };
}

#endif