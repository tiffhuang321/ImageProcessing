//
// Created by tiffh on 10/23/2024.
//

#ifndef IMAGE_H
#define IMAGE_H

#endif //IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char imageType;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;

    void Serialize(ofstream& file) {
        file.write(&idLength, 1);
        file.write(&colorMapType, 1);
        file.write(&imageType, 1);
        file.write(reinterpret_cast<char*>(&colorMapOrigin), 2);
        file.write(reinterpret_cast<char*>(&colorMapLength), 2);
        file.write(&colorMapDepth, 1);
        file.write(reinterpret_cast<char*>(&xOrigin), 2);
        file.write(reinterpret_cast<char*>(&yOrigin), 2);
        file.write(reinterpret_cast<char*>(&width), 2);
        file.write(reinterpret_cast<char*>(&height), 2);
        file.write(&bitsPerPixel, 1);
        file.write(&imageDescriptor, 1);
    }
    void readHeader(ifstream& inputFile) {
        inputFile.read(&idLength, 1);
        inputFile.read(&colorMapType, 1);
        inputFile.read(&imageType, 1);
        inputFile.read(reinterpret_cast<char*>(&colorMapOrigin), 2);
        inputFile.read(reinterpret_cast<char*>(&colorMapLength), 2);
        inputFile.read(&colorMapDepth, 1);
        inputFile.read(reinterpret_cast<char*>(&xOrigin), 2);
        inputFile.read(reinterpret_cast<char*>(&yOrigin), 2);
        inputFile.read(reinterpret_cast<char*>(&width), 2);
        inputFile.read(reinterpret_cast<char*>(&height), 2);
        inputFile.read(&bitsPerPixel, 1);
        inputFile.read(&imageDescriptor, 1);
    }
};

struct Image {
    vector<unsigned char> pixels;

    // methods
    inline void ReadPixels(ifstream& file);
    inline Image operator*(const Image& image);
    inline Image operator-(const Image& image);
    inline Image& operator+(const Image& image);
};

void Image::ReadPixels(ifstream& file) {
    unsigned char data;

    while (file.read(reinterpret_cast<char*>(&data), sizeof(data))) {
        pixels.push_back(data);
    }
}

Image Image::operator*(const Image& image) {
    Image newImage;

    for (int i = 0; i<image.pixels.size(); i++) {
        float layer1 = pixels[i]/255.0;
        float layer2 = image.pixels[i]/255.0;
        float newValue = (layer1 * layer2) * 255.0;

        newImage.pixels.push_back(newValue + 0.5f);
    }
    return newImage;
}

Image Image::operator-(const Image& image) {
    Image newImage;

    for (int i = 0; i < image.pixels.size(); i++ ) {
        int myPixel = pixels[i] - image.pixels[i];
        if (myPixel < 0) {
            newImage.pixels.push_back(0);
        }
        else {
            newImage.pixels.push_back(static_cast<unsigned char>(myPixel));
        }
    }
    return newImage;
}

Image& Image::operator+(const Image& image) {
    //Todo: revert back to original when it returned an Image rather than *this
    for (int i = 0; i < image.pixels.size(); i++ ) {
        int myPixel = pixels[i] + image.pixels[i];
        if (myPixel > 255) {
            pixels[i] = 255;
        }
        else if (myPixel < 0) {
            pixels[i] = 0;
        }
        else {
            pixels[i] = static_cast<unsigned char>(myPixel);
        }
    }
    return *this;

}
