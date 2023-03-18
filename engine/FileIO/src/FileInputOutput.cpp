#include "FileInputOutput.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "dependencies/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "dependencies/stb/stb_image_write.h"

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

    FileStatus FileInputOutput::loadImage(std::string filePath, mitevox::Image* image)
    {
        int initialImageFormat = 0;
        image->imageData = stbi_load(filePath.c_str(), &image->width, &image->height, &initialImageFormat, (int)mitevox::ImageFormat::RED_GREEN_BLUE_ALPHA);
        if (image->imageData == nullptr)
        {
            return FileStatus::ERROR;
        }
        image->filePath = filePath;
        image->format = mitevox::ImageFormat::RED_GREEN_BLUE_ALPHA;
        return FileStatus::READY;
    }

    void FileInputOutput::loadImage(std::string filePath, void** image, std::atomic<FileStatus>* fileStatus)
    {
        fileStatus->store(FileStatus::LOADING);
        mitevox::Image* imageObject = (mitevox::Image*)*image;
        FileStatus result = FileInputOutput::loadImage(filePath, imageObject);
        fileStatus->store(result);
    }

    FileStatus FileInputOutput::saveImage(mitevox::Image* image, std::string filePath)
    {
        int result = stbi_write_png(filePath.c_str(), image->width, image->height, (int)image->format, (const void*)image->imageData, 0);
        if (result == 0)
        {
            return FileStatus::ERROR;
        }
        return FileStatus::READY;
    }

    void FileInputOutput::saveImage(void** image, std::string filePath, std::atomic<FileStatus>* fileStatus)
    {
        fileStatus->store(FileStatus::SAVING);
        mitevox::Image* imageObject = (mitevox::Image*)*image;
        FileStatus result = FileInputOutput::saveImage(imageObject, filePath);
        fileStatus->store(result);
    }

    void FileInputOutput::freeImageData(mitevox::Image* image)
    {
        stbi_image_free(image->imageData);
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
