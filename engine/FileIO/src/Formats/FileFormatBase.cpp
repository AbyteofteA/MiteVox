#include "FileFormatBase.h"
#include "engine/FileIO/src/FileInputOutput.h"
#include "engine/FileIO/src/FileStatus.h"

#include <string>
#include <fstream>

namespace fileio
{
    FileStatus FileFormatBase::saveToFile(std::string filename)
    {
        safety::SafeByteArray fileData;
        serialize(&fileData);
        FileStatus fileStatus = FileInputOutput::saveBinary(&fileData, filename);
        if (fileStatus == FileStatus::READY)
        {
            _path = filename;
        }
        fileData.deallocate();
        return fileStatus;
    }

    FileStatus FileFormatBase::readFromFile(std::string filename)
    {
        safety::SafeByteArray fileData;
        FileStatus fileStatus = FileInputOutput::loadBinary(filename, &fileData);
        if (fileStatus == FileStatus::READY)
        {
            _path = filename;
            deserialize(&fileData);
        }
        fileData.deallocate();
        return fileStatus;
    }

    SerializationStatus FileFormatBase::serialize(safety::SafeByteArray* fileData)
    {
        return SerializationStatus::ERROR;
    }

    DeserializationStatus FileFormatBase::deserialize(safety::SafeByteArray* fileData)
    {
        return DeserializationStatus::ERROR;
    }
}