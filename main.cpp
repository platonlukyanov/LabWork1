#include <fstream>
#include <iostream>

#include "BMPFile.h"
#include "BMPPixelMatrix.h"
#include "bmpHeader.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "Please, specify action (available 'rotate-clockwise', "
                     "'rotate-anticlockwise') and after that source and target "
                     "files: ./main <action> <source> <target>\n";
        return 1;
    }
    std::string action = argv[1];
    std::string source = argv[2];
    std::string target = argv[3];

    BMPFile bmpFile(&source);
    if (action == "rotate-clockwise") {
        bmpFile.rotate90Degrees();
    }
    if (action == "rotate-anticlockwise") {
        bmpFile.rotateNegative90Degrees();
    }
    if (action == "gauss") {
        bmpFile.applyGaussianBlur();
    }
    bmpFile.write(&target);
}
