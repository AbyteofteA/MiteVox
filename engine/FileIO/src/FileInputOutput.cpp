
#include <iostream>
#include <thread>
#include <vector>

#include "FileInputOutput.h"

namespace fileio
{
    FileInputOutput fileInputOutput;

    FileInputOutput::FileInputOutput(uint16_t loaderThreadsCount)
    {
        _loaderThreadsCount = loaderThreadsCount;
    }

    FileInputOutput::~FileInputOutput()
    {

    }

    void FileInputOutput::loadBinary(std::string filename, void** result, std::atomic<FileStatus>* flag)
    {
        *result = nullptr;
        flag->store(FileStatus::LOADING);
        char* fileData = nullptr;

        FILE* file;
        fopen_s(&file, filename.c_str(), "rb");
        if (file == nullptr)
        {
            printf("\n ERROR! Cannot open the file.\n");
            printf("File: %s\n", filename.c_str());
            flag->store(FileStatus::ERROR);
            return;
        }
        else // read the file
        {
            // Determine the file size.
            int size = 1;
            fseek(file, 0, SEEK_END);
            size = ftell(file) + 1;
            fseek(file, 0, SEEK_SET);

            fileData = (char*)malloc(sizeof(char) * size);

            char tmp = 0;
            size = 0;
            while ((tmp = fgetc(file)) != EOF)
            {
                fileData[size] = tmp;
                size++;
            }
            fileData[size - 1] = '\0';

            fclose(file);
            (*result) = (void*)fileData;

            flag->store(FileStatus::READY);
            return;
        }
    }

    FileStatus FileInputOutput::loadBinary(std::string filePath, safety::SafeByteArray* fileData)
    {
        if (fileData == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        FILE* file;
        fopen_s(&file, filePath.c_str(), "rb");

        if (file == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        uint64_t fileSize = 0;
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        fileData->resize(fileSize);
        fread(fileData->getElementsArray(), (size_t)fileSize, (size_t)1, file);

        fclose(file);
        return FileStatus::READY;
    }

    void FileInputOutput::loadBinary(std::string filePath, safety::SafeByteArray* fileData, std::atomic<FileStatus>* fileStatus)
    {
        if (fileData == nullptr ||
            fileStatus == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return;
        }

        fileStatus->store(FileStatus::LOADING);
        FileStatus localFileStatus = loadBinary(filePath, fileData);
        fileStatus->store(localFileStatus);
    }

    FileStatus FileInputOutput::saveBinary(safety::SafeByteArray* fileData, std::string filePath)
    {
        if (fileData == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        FILE* file;
        fopen_s(&file, filePath.c_str(), "wb");

        if (file == nullptr)
        {
            printf("ERROR: Cannot write %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        fwrite(fileData->getElementsArray(), (size_t)fileData->getElementsCount(), (size_t)1, file);

        fclose(file);
        return FileStatus::READY;
    }

    FileStatus FileInputOutput::loadText(std::string filePath, std::string* text)
    {
        if (text == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        safety::SafeArray<uint8_t> fileData;
        FileStatus fileStatus = FileInputOutput::loadBinary(filePath, &fileData);

        if (fileStatus == FileStatus::ERROR)
        {
            return fileStatus;
        }

        uint32_t fileSize = fileData.getElementsCount() + 1;

        *text = std::string(fileSize, '\0');
        for (uint32_t i = 0; i < fileSize; ++i)
        {
            (*text)[i] = (char)fileData.getElement(i);
        }
        return fileStatus;
    }

    void FileInputOutput::loadText(std::string filePath, std::string* text, std::atomic<FileStatus>* fileStatus)
    {
        if (text == nullptr ||
            fileStatus == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return;
        }

        fileStatus->store(FileStatus::LOADING);
        FileStatus localFileStatus = loadText(filePath, text);
        fileStatus->store(localFileStatus);
    }

    FileStatus FileInputOutput::saveText(std::string* text, std::string filePath)
    {
        if (text == nullptr)
        {
            printf("ERROR: Cannot open %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        FILE* file;
        fopen_s(&file, filePath.c_str(), "w");

        if (file == nullptr)
        {
            printf("ERROR: Cannot write %s\n", filePath.c_str()); // TODO: use logger for errors
            return FileStatus::ERROR;
        }

        fwrite(text->c_str(), text->size(), (size_t)1, file);

        fclose(file);
        return FileStatus::READY;
    }
}
