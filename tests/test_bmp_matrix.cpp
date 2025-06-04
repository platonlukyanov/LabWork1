#include <gtest/gtest.h>
#include "BMPPixelMatrix.h"

class BMPPixelMatrixTest : public ::testing::Test {
protected:
    void SetUp() override {
        matrix = new BMPPixelMatrix(3, 3, 24);
        
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                Pixel pixel;
                pixel.red = x * 50;
                pixel.green = y * 50;
                pixel.blue = (x + y) * 25;
                matrix->setPixel(x, y, pixel);
            }
        }
    }

    void TearDown() override {
        delete matrix;
    }

    BMPPixelMatrix* matrix;
};

TEST_F(BMPPixelMatrixTest, MatrixDimensions) {
    EXPECT_EQ(matrix->getWidth(), 3);
    EXPECT_EQ(matrix->getHeight(), 3);
}

TEST_F(BMPPixelMatrixTest, Rotate90Degrees) {
    Pixel original[3][3];
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            original[y][x] = matrix->getPixel(x, y);

    matrix->rotate90Degrees();

    EXPECT_EQ(matrix->getWidth(), 3);
    EXPECT_EQ(matrix->getHeight(), 3);

    for (int y = 0; y < matrix->getHeight(); y++)
        for (int x = 0; x < matrix->getWidth(); x++) {
            Pixel rotated = matrix->getPixel(x, y);
            Pixel expected = original[x][3 - 1 - y];
            EXPECT_EQ(rotated.red, expected.red);
            EXPECT_EQ(rotated.green, expected.green);
            EXPECT_EQ(rotated.blue, expected.blue);
        }
}

TEST_F(BMPPixelMatrixTest, RotateNegative90Degrees) {
    Pixel original[3][3];
    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
            original[y][x] = matrix->getPixel(x, y);

    matrix->rotateNegative90Degrees();

    EXPECT_EQ(matrix->getWidth(), 3);
    EXPECT_EQ(matrix->getHeight(), 3);

    for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++) {
            Pixel rotated = matrix->getPixel(x, y);
            Pixel expected = original[x][y];
            EXPECT_EQ(rotated.red, expected.red);
            EXPECT_EQ(rotated.green, expected.green);
            EXPECT_EQ(rotated.blue, expected.blue);
        }
}

TEST_F(BMPPixelMatrixTest, GaussianBlur) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Pixel pixel;
            pixel.red = x * 100;
            pixel.green = y * 100;
            pixel.blue = (x + y) * 50;
            matrix->setPixel(x, y, pixel);
        }
    }

    Pixel original[3][3];
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            original[y][x] = matrix->getPixel(x, y);
        }
    }

    matrix->applyGaussianBlur();

    bool valuesChanged = false;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Pixel blurred = matrix->getPixel(x, y);
            if (blurred.red != original[y][x].red ||
                blurred.green != original[y][x].green ||
                blurred.blue != original[y][x].blue) {
                valuesChanged = true;
                break;
            }
        }
        if (valuesChanged) break;
    }
    EXPECT_TRUE(valuesChanged);

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            Pixel pixel = matrix->getPixel(x, y);
            EXPECT_GE(pixel.red, 0);
            EXPECT_LE(pixel.red, 255);
            EXPECT_GE(pixel.green, 0);
            EXPECT_LE(pixel.green, 255);
            EXPECT_GE(pixel.blue, 0);
            EXPECT_LE(pixel.blue, 255);
        }
    }
} 