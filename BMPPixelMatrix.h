#ifndef BMPPIXELMATRIX_H
#define BMPPIXELMATRIX_H

#include <cstdint>

#include "Pixel.h"

class BMPPixelMatrix {
    public:
        BMPPixelMatrix(int width, int height, int bytesPerPixel);
        ~BMPPixelMatrix();

        void loadPixelMatrix(uint8_t* rawPixels);
        uint8_t* convertMatrixToRawPixels();
        void rotate90Degrees();
        void rotateNegative90Degrees();
        int getWidth();
        int getHeight();
        void applyGaussianBlur();

    private:
        int _width;
        int _height;
        int _bytesPerPixel;
        Pixel** _getEmptyMatrix(int width, int height);
        void _clearMatrix();
        Pixel** matrix;

        void loadPixelMatrix(uint8_t* rawPixels, int bitPerPixel);
};

#endif