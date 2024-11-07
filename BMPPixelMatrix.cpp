/* Platon Lukyanov st128133@student.spbu.ru
 * Lab Work 1
*/

#include "BMPPixelMatrix.h"

#include <algorithm>
#include <cstring>
#include <utility>

BMPPixelMatrix::BMPPixelMatrix(int width, int height, int bytesPerPixel)
    : _width(width), _height(height), _bytesPerPixel(bytesPerPixel) {
    matrix = _getEmptyMatrix(width, height);
}

BMPPixelMatrix::~BMPPixelMatrix() {
    for (int i = 0; i < _height; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

int BMPPixelMatrix::getWidth() {
    return _width;
}

int BMPPixelMatrix::getHeight() {
    return _height;
}

void BMPPixelMatrix::loadPixelMatrix(uint8_t* rawPixels) {
    loadPixelMatrix(rawPixels, _bytesPerPixel);
}

void BMPPixelMatrix::loadPixelMatrix(uint8_t* rawPixels, int bitPerPixel) {
    int bytesPerPixel = (bitPerPixel / 8);
    int rowSize = ((bytesPerPixel * _width + 3) &
                   ~3);  // Round up to nearest multiple of 4

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            int index = ((_height - 1 - y) * rowSize + x * bytesPerPixel);
            matrix[y][x].blue = rawPixels[index];
            matrix[y][x].green = rawPixels[index + 1];
            matrix[y][x].red = rawPixels[index + 2];
        }
    }
}

uint8_t* BMPPixelMatrix::convertMatrixToRawPixels() {
    int bytesPerPixel = (_bytesPerPixel / 8);
    int rowSize = ((bytesPerPixel * _width + 3) &
                   ~3);  // Round up to nearest multiple of 4

    uint8_t* rawPixels = new uint8_t[_height * rowSize];

    memset(rawPixels, 0, _height * rowSize);  // Initialize to zero

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            int index = (y * rowSize + x * bytesPerPixel);
            rawPixels[index] = matrix[_height - 1 - y][x].blue;
            rawPixels[index + 1] = matrix[_height - 1 - y][x].green;
            rawPixels[index + 2] = matrix[_height - 1 - y][x].red;
        }
        // Add padding if necessary
        for (int p = bytesPerPixel * _width; p < rowSize; ++p) {
            rawPixels[y * rowSize + p] = 0;
        }
    }

    return rawPixels;
}

Pixel** BMPPixelMatrix::_getEmptyMatrix(int width, int height) {
    Pixel** newMatrix = new Pixel*[height];
    for (int i = 0; i < height; ++i) {
        newMatrix[i] = new Pixel[width];
    }
    return newMatrix;
}

void BMPPixelMatrix::_clearMatrix() {
    for (int i = 0; i < _height; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void BMPPixelMatrix::rotateNegative90Degrees() {
    Pixel** rotatedMatrix = _getEmptyMatrix(_height, _width);

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            rotatedMatrix[x][y] = matrix[y][x];
        }
    }
    _clearMatrix();
    std::swap(_width, _height);
    matrix = rotatedMatrix;
}

void BMPPixelMatrix::rotate90Degrees() {
    Pixel** rotatedMatrix = _getEmptyMatrix(_height, _width);

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            rotatedMatrix[x][_height - 1 - y] = matrix[y][x];
        }
    }
    _clearMatrix();
    std::swap(_width, _height);
    matrix = rotatedMatrix;
}

void BMPPixelMatrix::applyGaussianBlur() {
    const int kernelSize = 5;
    const float sigma = 1.0f;
    const float kernel[kernelSize][kernelSize] = {{1, 4, 6, 4, 1},
                                                  {4, 16, 24, 16, 4},
                                                  {6, 24, 36, 24, 6},
                                                  {4, 16, 24, 16, 4},
                                                  {1, 4, 6, 4, 1}};

    float kernelSum = 0.0f;
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernelSum += kernel[i][j];
        }
    }

    Pixel** blurredMatrix = _getEmptyMatrix(_width, _height);

    for (int y = 0; y < _height; ++y) {
        for (int x = 0; x < _width; ++x) {
            float blue = 0.0f;
            float green = 0.0f;
            float red = 0.0f;

            for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ++ky) {
                for (int kx = -kernelSize / 2; kx <= kernelSize / 2; ++kx) {
                    int pixelY = std::clamp(y + ky, 0, _height - 1);
                    int pixelX = std::clamp(x + kx, 0, _width - 1);
                    blue += matrix[pixelY][pixelX].blue *
                            (kernel[ky + kernelSize / 2][kx + kernelSize / 2] /
                             kernelSum);
                    green += matrix[pixelY][pixelX].green *
                             (kernel[ky + kernelSize / 2][kx + kernelSize / 2] /
                              kernelSum);
                    red += matrix[pixelY][pixelX].red *
                           (kernel[ky + kernelSize / 2][kx + kernelSize / 2] /
                            kernelSum);
                }
            }

            blurredMatrix[y][x].blue =
                static_cast<uint8_t>(std::clamp(blue, 0.0f, 255.0f));
            blurredMatrix[y][x].green =
                static_cast<uint8_t>(std::clamp(green, 0.0f, 255.0f));
            blurredMatrix[y][x].red =
                static_cast<uint8_t>(std::clamp(red, 0.0f, 255.0f));
        }
    }

    _clearMatrix();
    matrix = blurredMatrix;
}
