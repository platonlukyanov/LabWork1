#include <fstream>
#include <iostream>

#include "bmpHeader.h"

struct Pixel {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
};

class BMPPixelMatrix {
    public:
        BMPPixelMatrix(uint8_t* rawPixels, int width, int height,
                       int bytesPerPixel)
            : _width(width), _height(height) {
            loadPixelMatrix(rawPixels, bytesPerPixel);
        }
        ~BMPPixelMatrix() {
            delete[] matrix;
        }

        uint8_t* convertMatrixToRawPixels() {
            int bytesPerPixel = (bytesPerPixel / 8);
            int rowSize = ((bytesPerPixel * _width + 3) &
                           ~3);  // Round up to nearest multiple of 4

            uint8_t* rawPixels = new uint8_t[_height * rowSize];

            for (int y = 0; y < _height; ++y) {
                for (int x = 0; x < _width; ++x) {
                    int index = (y * rowSize + x * bytesPerPixel);
                    rawPixels[index] = matrix[_height - 1 - y][x].blue;
                    rawPixels[index + 1] = matrix[_height - 1 - y][x].green;
                    rawPixels[index + 2] = matrix[_height - 1 - y][x].red;
                }
                // Add padding if necessary
                for (int p = bytesPerPixel * _width; p < rowSize; ++p) {
                    rawPixels[y * rowSize + p] =
                        0;  // Padding bytes set to zero
                }
            }

            return rawPixels;
        }

    private:
        int _width;
        int _height;
        Pixel** matrix;
        void loadPixelMatrix(uint8_t* rawPixels, int bitPerPixel) {
            Pixel** matrix = new Pixel*[_height];
            for (int i = 0; i < _height; ++i) {
                matrix[i] = new Pixel[_width];
            }

            int bytesPerPixel = (bitPerPixel / 8);
            int rowSize = ((bytesPerPixel * _width + 3) &
                           ~3);  // Round up to nearest multiple of 4

            for (int y = 0; y < _height; ++y) {
                for (int x = 0; x < _width; ++x) {
                    int index =
                        ((_height - 1 - y) * rowSize + x * bytesPerPixel);
                    matrix[y][x].blue = rawPixels[index];
                    matrix[y][x].green = rawPixels[index + 1];
                    matrix[y][x].red = rawPixels[index + 2];
                }
            }
        }
};

class BMPFile {
    public:
        BMPFile(const char* filename) : _filename(filename) { load(); }
        ~BMPFile() {
            delete[] pixels;
            delete pixelMatrix;
        }
        int getWidth() { return _infoHeader.biWidth; }
        int getHeight() { return _infoHeader.biHeight; }
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
            file.write(reinterpret_cast<const char*>(&fileHeader),
                       sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader),
                       sizeof(infoHeader));
            file.write(reinterpret_cast<const char*>(pixels), getImageSize());
            file.close();
        }

    private:
        uint8_t* pixels;
        const char* _filename;
        BITMAPFILEHEADER _fileHeader;
        BITMAPINFOHEADER _infoHeader;
        static const uint16_t bmInLittleEndian = 0x4D42;
        BMPPixelMatrix* pixelMatrix;

        void setPixelMatrix(BMPPixelMatrix* matrix) { pixelMatrix = matrix; }

        void load() {
            std::ifstream file(_filename, std::ios::binary);

            if (!file) {
                std::cerr << "Unable to open file!" << std::endl;
                return;
            }

            file.read(reinterpret_cast<char*>(&_fileHeader),
                      sizeof(_fileHeader));
            file.read(reinterpret_cast<char*>(&_infoHeader),
                      sizeof(_infoHeader));

            if (_fileHeader.bfType != bmInLittleEndian) {
                std::cerr << "Not a BMP file!" << std::endl;
                return;
            }
            int width = getWidth();
            int height = getHeight();
            int imageSize = getImageSize();
            pixels = new uint8_t[imageSize];

            file.read(reinterpret_cast<char*>(pixels), imageSize);
            setPixelMatrix(new BMPPixelMatrix(pixels, getWidth(), getHeight(),
                                              _infoHeader.biBitPerPixel));
            file.close();
        }

        int getImageSize() {
            size_t imageSize = _infoHeader.biImageDataSize;
            if (imageSize == 0) {
                imageSize = _infoHeader.biWidth * _infoHeader.biHeight *
                            (_infoHeader.biBitPerPixel / 8);
            }
            return imageSize;
        }
};

int main() {
    std::cout << "Hello, world!\n";
    BMPFile bmpFile("example.bmp");
    std::cout << bmpFile.getWidth() << " and " << bmpFile.getHeight() << "\n";
    bmpFile.write("example3.bmp");
}
