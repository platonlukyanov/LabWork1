#include <iostream>
#include <fstream>
#include "bmpHeader.h";

void readBmp(const char* filename) {
   std::ifstream file(filename, std::ios::binary); 
   BITMAPFILEHEADER bmFileHeader;
   BITMAPINFOHEADER bmInfoHeader;

   if (!file) {
    std::cerr << "Unable to open file!" << std::endl;
    return;
   }

   file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));
   file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

   const uint16_t bmInLittleEndian = 0x4D42;
   if (bmFileHeader.bfType != bmInLittleEndian) {
    std::cerr << "Not a BMP file!" << std::endl;
    return;
   }

//    std::cout << "Width: " << bmInfoHeader.biWidth 
//    << ", Height: " << bmInfoHeader.biHeight << ", Bit Count: "
//    << bmInfoHeader.biBitPerPixel;
    size_t imageSize = bmInfoHeader.biImageDataSize;
    if (imageSize == 0) {
        imageSize = bmInfoHeader.biWidth * bmInfoHeader.biHeight * (bmInfoHeader.biNumberOfColorPlanes / 8);
    }
    std::cout << "Size: " << imageSize;

    auto rawPixels = new uint8_t[imageSize];

    file.seekg(bmFileHeader.bfOffsetBits);
    file.read(reinterpret_cast<char*>(rawPixels), imageSize);
    delete[] rawPixels;
}

int main() {
    std::cout << "Hello, world!\n";
    readBmp("sample.bmp");
}
