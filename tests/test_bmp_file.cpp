/* Platon Lukyanov st128133@student.spbu.ru
 * Lab Work 1
 */
#include <gtest/gtest.h>

#include <fstream>

#include "BMPFile.h"

class BMPFileTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::string testFilename = "test.bmp";
            createTestBMPFile(testFilename);
            bmpFile = new BMPFile(&testFilename);
        }

        void TearDown() override {
            delete bmpFile;

            std::remove("test.bmp");
            std::remove("output.bmp");
        }

        void createTestBMPFile(const std::string& filename) {
            std::ofstream file(filename, std::ios::binary);

            // Создаем заголовок файла
            BITMAPFILEHEADER fileHeader;
            fileHeader.bfType = 0x4D42;  // 'BM'
            fileHeader.bfSize = 54 + 3 * 3 * 3;
            fileHeader.bfReserved1 = 0;
            fileHeader.bfReserved2 = 0;
            fileHeader.bfOffsetBits = 54;

            BITMAPINFOHEADER infoHeader;
            infoHeader.biSize = 40;
            infoHeader.biWidth = 3;
            infoHeader.biHeight = 3;
            infoHeader.biNumberOfColorPlanes = 1;
            infoHeader.biBitPerPixel = 24;
            infoHeader.biCompressionType = 0;
            infoHeader.biImageDataSize = 3 * 3 * 3;
            infoHeader.biXPixelsPerMeter = 0;
            infoHeader.biYPixelsPerMeter = 0;
            infoHeader.biNumberOfColorsUsed = 0;
            infoHeader.biImportantColors = 0;

            file.write(reinterpret_cast<const char*>(&fileHeader),
                       sizeof(fileHeader));
            file.write(reinterpret_cast<const char*>(&infoHeader),
                       sizeof(infoHeader));

            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    uint8_t pixel[3] = {
                        static_cast<uint8_t>(x * 50),       // B
                        static_cast<uint8_t>(y * 50),       // G
                        static_cast<uint8_t>((x + y) * 25)  // R
                    };
                    file.write(reinterpret_cast<const char*>(pixel), 3);
                }
            }

            file.close();
        }

        BMPFile* bmpFile;
};

TEST_F(BMPFileTest, FileDimensions) {
    EXPECT_EQ(bmpFile->getWidth(), 3);
    EXPECT_EQ(bmpFile->getHeight(), 3);
}

TEST_F(BMPFileTest, Rotate90Degrees) {
    int originalWidth = bmpFile->getWidth();
    int originalHeight = bmpFile->getHeight();

    bmpFile->rotate90Degrees();

    EXPECT_EQ(bmpFile->getWidth(), originalHeight);
    EXPECT_EQ(bmpFile->getHeight(), originalWidth);
}

TEST_F(BMPFileTest, RotateNegative90Degrees) {
    int originalWidth = bmpFile->getWidth();
    int originalHeight = bmpFile->getHeight();

    bmpFile->rotateNegative90Degrees();

    // Checking dimensions changing places
    EXPECT_EQ(bmpFile->getWidth(), originalHeight);
    EXPECT_EQ(bmpFile->getHeight(), originalWidth);
}

TEST_F(BMPFileTest, WriteFile) {
    std::string outputFilename = "output.bmp";
    bmpFile->write(&outputFilename);

    // Checking if file was created
    std::ifstream file(outputFilename, std::ios::binary);
    EXPECT_TRUE(file.is_open());
    file.close();
}

TEST_F(BMPFileTest, GaussianBlur) {
    bmpFile->applyGaussianBlur();

    EXPECT_EQ(bmpFile->getWidth(), 3);
    EXPECT_EQ(bmpFile->getHeight(), 3);
}

TEST_F(BMPFileTest, Headers) {
    // Checking if headers are loaded
    BITMAPFILEHEADER fileHeader = bmpFile->getFileHeader();
    BITMAPINFOHEADER infoHeader = bmpFile->getInfoHeader();

    EXPECT_EQ(fileHeader.bfType, 0x4D42);  // 'BM'
    EXPECT_EQ(infoHeader.biWidth, 3);
    EXPECT_EQ(infoHeader.biHeight, 3);
    EXPECT_EQ(infoHeader.biBitPerPixel, 24);
}