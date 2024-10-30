#include <iostream>
#include <fstream>
#include "bmpHeader.h"

class BMPFile {
    public:
        BMPFile(const char* filename) : _filename(filename) {
            load();
        }
        ~BMPFile() {
            delete[] pixels;
        }
        int getWidth() {
            return _infoHeader.biWidth;
        }
        int getHeight() {
            return _infoHeader.biHeight;
        }
        void write(const char* newFilename) {
            std::ofstream file(newFilename, std::ios::binary);
 
            int height = getHeight();
            int width = getWidth();

            BITMAPFILEHEADER fileHeader = _fileHeader;
            BITMAPINFOHEADER infoHeader = _infoHeader;
            
            if (!file) {
                std::cerr << "Unable to open file!" << std::endl;
                return;
            }
            file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));
            file.write(reinterpret_cast<const char*>(pixels), getImageSize());
            file.close();
        }
    private:
        uint8_t* pixels;
        const char* _filename;
        BITMAPFILEHEADER _fileHeader;
        BITMAPINFOHEADER _infoHeader;
        static const uint16_t bmInLittleEndian = 0x4D42;
 
        void load() {
            std::ifstream file(_filename, std::ios::binary); 

            if (!file) {
                std::cerr << "Unable to open file!" << std::endl;
                return;
            }

            file.read(reinterpret_cast<char*>(&_fileHeader), sizeof(_fileHeader));
            file.read(reinterpret_cast<char*>(&_infoHeader), sizeof(_infoHeader));

            if (_fileHeader.bfType != bmInLittleEndian) {
                std::cerr << "Not a BMP file!" << std::endl;
                return;
            }
            int width = getWidth();
            int height = getHeight();
            int imageSize = getImageSize();
            pixels = new uint8_t[imageSize];

            file.read(reinterpret_cast<char*>(pixels), imageSize);
            file.close();
        }

        int getImageSize() {
            size_t imageSize = _infoHeader.biImageDataSize;
            if (imageSize == 0) {
                imageSize = _infoHeader.biWidth * _infoHeader.biHeight * (_infoHeader.biBitPerPixel / 8);
            }
            return imageSize;
        }
};

int main() {
    std::cout << "Hello, world!\n";
    BMPFile bmpFile("sample.bmp");
    std::cout << bmpFile.getWidth() << " and " << bmpFile.getHeight();
    bmpFile.write("sample2.bmp");
}
