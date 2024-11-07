/* Platon Lukyanov st128133@student.spbu.ru
 * Lab Work 1
*/

#include "BMPFile.h"

BMPFile::BMPFile(const std::string* filename) : _filename(filename) {
    load();
}

BMPFile::~BMPFile() {
    delete[] pixels;
    delete pixelsMatrix;
}

int BMPFile::getWidth() {
    return _infoHeader.biWidth;
}

int BMPFile::getHeight() {
    return _infoHeader.biHeight;
}

void BMPFile::write(const std::string* newFilename) {
    std::ofstream file(newFilename->c_str(), std::ios::binary);

    if (!file) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    BITMAPFILEHEADER fileHeader = _fileHeader;
    BITMAPINFOHEADER infoHeader = _infoHeader;

    int height = pixelsMatrix->getHeight();
    int width = pixelsMatrix->getWidth();

    infoHeader.biWidth = width;
    infoHeader.biHeight = height;

    uint8_t* pixelsForWriting = pixelsMatrix->convertMatrixToRawPixels();

    file.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    file.write(reinterpret_cast<const char*>(pixelsForWriting), getImageSize());

    file.close();
}

void BMPFile::rotate90Degrees() {
    if (!pixelsMatrix) {
        std::cerr << "Can't rotate inexistent matrix\n";
        return;
    }

    pixelsMatrix->rotate90Degrees();
}

void BMPFile::rotateNegative90Degrees() {
    if (!pixelsMatrix) {
        std::cerr << "Can't rotate inexistent matrix\n";
        return;
    }

    pixelsMatrix->rotateNegative90Degrees();
}

BITMAPINFOHEADER BMPFile::getInfoHeader() {
    return _infoHeader;
}

BITMAPFILEHEADER BMPFile::getFileHeader() {
    return _fileHeader;
}

void BMPFile::load() {
    std::ifstream file(_filename->c_str(), std::ios::binary);

    if (!file) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&_fileHeader), sizeof(_fileHeader));

    if (_fileHeader.bfType != bmInLittleEndian) {
        std::cerr << "Not a BMP file!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(&_infoHeader), sizeof(_infoHeader));

    int width = getWidth();
    int height = getHeight();
    int paddedRowSize = (width * (_infoHeader.biBitPerPixel / 8) + 3) & ~3;

    pixels = new uint8_t[paddedRowSize * height];

    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(pixels + i * paddedRowSize),
                  paddedRowSize);
    }

    pixelsMatrix =
        new BMPPixelMatrix(getWidth(), getHeight(), _infoHeader.biBitPerPixel);
    pixelsMatrix->loadPixelMatrix(pixels);

    file.close();
}

void BMPFile::applyGaussianBlur() {
    pixelsMatrix->applyGaussianBlur();
}

int BMPFile::getImageSize() {
    int paddedRowSize =
        (_infoHeader.biWidth * (_infoHeader.biBitPerPixel / 8) + 3) & ~3;
    return paddedRowSize * _infoHeader.biHeight;
}