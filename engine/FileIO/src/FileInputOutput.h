#ifndef FILEINPUTOUTPUT_H
#define FILEINPUTOUTPUT_H

#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <cstdint>

#include "FileStatus.h"
#include "engine/CodeSafety/src/SafeArray.h"
#include "engine/MiteVox/src/Material/Image.h"

namespace fileio
{
    /// <summary>
    /// Declares file input/output API.
    /// </summary>
    class FileInputOutput
    {
    public:

        FileInputOutput(uint16_t loaderThreadsCount = 1);
        ~FileInputOutput();

        /// Reads a file from filename and assigns the result with the pointer to the read file.
        /// Assignes the flag with 1 when the file is loaded.

        static void loadBinary(std::string filename, void** result, std::atomic<FileStatus>* flag);
        static FileStatus loadBinary(std::string filePath, safety::SafeByteArray* fileData);
        static void loadBinary(std::string filePath, safety::SafeByteArray* fileData, std::atomic<FileStatus>* fileStatus);
        static FileStatus saveBinary(safety::SafeByteArray* fileData, std::string filePath);
        static void saveBinary(safety::SafeByteArray* fileData, std::string filePath, std::atomic<FileStatus>* fileStatus);

        static FileStatus loadImage(std::string filePath, mitevox::Image* image);
        static void loadImage(std::string filePath, void** image, std::atomic<FileStatus>* fileStatus);
        static FileStatus saveImage(mitevox::Image* image, std::string filePath);
        static void saveImage(void** image, std::string filePath, std::atomic<FileStatus>* fileStatus);
        static void freeImageData(mitevox::Image* image);

        static FileStatus loadText(std::string filePath, std::string* text);
        static void loadText(std::string filePath, std::string* text, std::atomic<FileStatus>* fileStatus);
        static FileStatus saveText(std::string* text, std::string filePath);
        static void saveText(std::string* text, std::string filePath, std::atomic<FileStatus>* fileStatus);

        /*void loadAsynchronouly(safety::SafeArray<uint8_t> fileData,
                               std::string filePath);

        safety::SafeArray<uint8_t> loadAsynchronouly(std::string filePath);

        void saveAsynchronouly(safety::SafeArray<uint8_t> fileData, std::string filePath);

        void update();*/

    private:

        uint16_t _loaderThreadsCount;
    };

    extern FileInputOutput fileInputOutput;
}

#endif