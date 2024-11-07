#ifndef BMPFILE_H
#define BMPFILE_H

#include <iostream>
#include <fstream>
#include <cstdint>
#include "BMPPixelMatrix.h"
#include "bmpHeader.h"
#include <string>

class BMPFile {
public:
    BMPFile(const std::string* filename);
    ~BMPFile();
    
    int getWidth();
    int getHeight();
    
    void write(const std::string* newFilename);
    
    void rotate90Degrees();
    void rotateNegative90Degrees();
    void applyGaussianBlur();
    
    BITMAPINFOHEADER getInfoHeader();
    BITMAPFILEHEADER getFileHeader();

private:
    uint8_t* pixels;
    const std::string* _filename;
    
    BITMAPFILEHEADER _fileHeader;
    BITMAPINFOHEADER _infoHeader;

    static const uint16_t bmInLittleEndian = 0x4D42; // 'BM'
    
    BMPPixelMatrix* pixelsMatrix = nullptr;

    void load();
    
    int getImageSize();
};

#endif 