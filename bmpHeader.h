#ifndef BITMAP_HEADER
#define BITMAP_HEADER
#include <iostream>
#include <fstream>
#include <cstdint>

// bf=bitmap file
// bi=bitmap info
#pragma pack(push, 1) 
struct BITMAPFILEHEADER {
    uint16_t bfType;    
    uint32_t bfSize;   
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffsetBits;
};

struct BITMAPINFOHEADER {
    uint32_t biSize; 
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biNumberOfColorPlanes;
    uint16_t biBitPerPixel;
    uint32_t biCompressionType;   //  (0 = none)
    uint32_t biImageDataSize;
    int32_t biXPixelsPerMeter;
    int32_t biYPixelsPerMeter;
    uint32_t biNumberOfColorsUsed;
    uint32_t biImportantColors;
};
#pragma pack(pop)
#endif
