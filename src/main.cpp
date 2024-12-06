#include <algorithm>
#include <complex>
#include <iostream>
#include <fstream>
#include "Image.h"
#include <string>
#include <cstring>

using namespace std;

void writeFile(Header& header, Image& image, const string& filename);
void readFile(Header& header, Image& image, const string& filename);

void Multiply(Image& tracking, const Image& arg);
void Subtract(Image& tracking, const Image& arg);
void Overlay(Image& tracking, const Image& arg);
void Screen(Image& tracking, const Image& arg);
void Combine(Image& red, const Image& green, const Image& blue);
void Flip(Image& tracking);
Image channelImage(const Image& image, int value, int channel);
void Scale(Image& image, int channel, const int& scale);
void onlygreen(Image& green);
void onlyblue(Image& blue);
void onlyred(Image& red);
void addred(Image& add, const string& value);
void addgreen(Image& add, const string& value);
void addblue(Image& add, const string& value);
void scalered(Image& scale, const string& value);
void scalegreen(Image& scale, const string& value);
void scaleblue(Image& scale, const string& value);

bool checkMethod(const string& method);
bool trystoi(const string& str);


int main(int argc, const char* argv[])
{
    if (argc == 1 || strcmp(argv[1], "--help") == 0) {
        cout << "Project 2: Image Processing, Fall 2024" << "\n" << endl;
        cout << "Usage:\n\t./project2.out [output] [firstImage] [method] [...]" << endl;
    }
    else {
        // checking the number of arguments
        if (argc < 3) {
            cout << "Invalid file name." << endl;
        }
        else {
            const string ofile = argv[1];
            const string ifile = argv[2];

            ifstream file(ifile, ios_base::binary);
            // checking file
            if (ofile.find(".tga")==string::npos || ifile.find(".tga") == string::npos) {
                cout << "Invalid file name." << endl;
            }
            else if (!file) {
                cout << "File does not exist." << endl;
            }
            else {
                file.close();

                if (argc > 3) {

                    string method = argv[3];

                    // processing is happening in this if block

                    if (checkMethod(method)) { // TODO: only checks the first method, check the rest in the while loop
                        Header header; // loading the image
                        Image tracking;

                        readFile(header, tracking, ifile);
                        int index = 3;

                        while (index < argc) { // keep checking for methods while there are more args
                            method = argv[index];

                           if (!checkMethod(method)) {
                             cout << "Invalid method name." << endl;
                             return 1;
                           }

                            if (method == "flip" || method == "onlygreen" || // these methods don't need to check for additional arguments
                                method == "onlyred" || method == "onlyblue") {
                                string channel;
                                if (index > 3) {
                                    if (method == "flip") {
                                        cout << "... and flipping output of previous step ..." << endl;
                                        Flip(tracking);

                                    } else {

                                        if (method == "onlygreen") {
                                            channel = "green";
                                            onlygreen(tracking);
                                        }
                                        if (method == "onlyred") {
                                            channel = "red";
                                            onlyred(tracking);
                                        }
                                        if (method == "onlyblue") {
                                            channel = "blue";
                                            onlyblue(tracking);
                                        }
                                        cout << "... and retrieving channel " << channel << " from " << ifile << " ..." << endl;
                                    }
                                }
                                else {
                                    if (method == "flip") {
                                        cout << "Flipping " << ifile << "..." << endl;
                                        Flip(tracking);
                                    } else {
                                        if (method == "onlygreen") {
                                            channel = "green";
                                            onlygreen(tracking);
                                        }
                                        if (method == "onlyred") {
                                            channel = "red";
                                            onlyred(tracking);
                                        }
                                        if (method == "onlyblue") {
                                            channel = "blue";
                                            onlyblue(tracking);
                                        }
                                        cout << "Retrieving channel " << channel << " from " << ifile << " ..." << endl;
                                    }
                                }

                            }
                            else { // for all other methods
                                if (index + 1 >= argc) { // check for additional argument passed after each method name
                                    cout << "Missing argument." << endl;
                                    return 1;
                                }

                                if(method == "combine") {
                                    if (index + 2 >= argc) { // assumes that combine will only be used once
                                        // additional check for combine's 2 args
                                        cout << "Missing argument." << endl;
                                        return 1;
                                    }
                                    string greenvalue = argv[index + 1];
                                    string bluevalue = argv[index + 2];

                                    // verifying file name
                                    if (greenvalue.find(".tga") == string::npos || bluevalue.find(".tga") == string::npos) {
                                        cout << "Invalid argument, invalid file name." << endl;
                                        return 1;
                                    }

                                    // verifying file exists
                                    ifstream bluefile(bluevalue, ios_base::binary);
                                    ifstream greenfile(greenvalue, ios_base::binary);

                                    if(!bluefile || !greenfile) {
                                      cout << "Invalid argument, file does not exist." << endl;
                                      return 1;
                                    }

                                    bluefile.close();
                                    greenfile.close();

                                    Image green;
                                    Image blue;

                                    readFile(header, green, greenvalue);
                                    readFile(header, blue, bluevalue);

                                    if (index == 3) {
                                        cout << "Combining channels of " << ifile << ", " << greenvalue << ", and " << bluevalue << "." << endl;
                                        Combine (tracking, green, blue);
                                    }
                                    else {
                                        cout << "... and combining channels of " << ifile << ", " << greenvalue << ", and " << bluevalue << "." << endl;
                                    }

                                    index += 2;
                                }
                                else if (method == "scalered" || method == "scalegreen" || method == "scaleblue"
                                    || method == "addred" || method == "addgreen" || method == "addblue") {

                                    string arg = argv[index + 1]; // arg = the number after the method
                                    string channel; // for the purpose of printing

                                    if (!trystoi(arg)) {
                                        cout << "Invalid argument, expected number." << endl;
                                        return 1;
                                    }
                                    // printing statements


                                    // tasks
                                    if (method == "scalered") { scalered(tracking, arg); channel = "red"; }
                                    if (method == "scalegreen") { scalegreen(tracking, arg); channel = "green";}
                                    if (method == "scaleblue") { scaleblue(tracking, arg); channel = "blue"; }
                                    if (method == "addred") { addred(tracking, arg); channel = "red"; }
                                    if (method == "addgreen") { addgreen(tracking, arg); channel = "green";}
                                    if (method == "addblue") { addblue(tracking, arg); channel = "blue"; }

                                    if (index == 3) {
                                        if (method == "scalered" || method == "scaleblue" || method == "scalegreen") {
                                            cout << "Scaling the " << channel << " of " << ofile << " by " << arg << " ..." << endl;
                                        }
                                        else {
                                            cout << "Adding " << arg << " to the " << channel << " channel of " << ofile << " ..." << endl;
                                        }
                                    }
                                    else {
                                        if (method == "scalered" || method == "scaleblue" || method == "scalegreen") {
                                            cout << "... and scaling the " << channel << " channel of the previous step by " << arg << " ..." << endl;
                                        }
                                        else {
                                            cout << "... and adding " << arg << " to the " << channel << " channel of previous step ..." << endl;
                                        }
                                    }

                                    index ++;
                                }
                                else {

                                    string arg = argv[index + 1];
                                    string Method;

                                    if(arg.find(".tga") == string::npos) {
                                        cout << "Invalid argument, invalid file name." << endl;
                                        return 1;
                                    }

                                    ifstream argument(arg, ios_base::binary);
                                    if (!argument) {
                                      cout << "Invalid argument, file does not exist." << endl;
                                      return 1;
                                    }

                                    argument.close();

                                    Image image;
                                    readFile(header, image, arg);



                                    if (method == "multiply") { Multiply(tracking, image); Method = "Multiplying"; }
                                    if (method == "subtract") { Subtract(tracking, image); Method = "Subtracting"; }
                                    if (method == "overlay") { Overlay(tracking, image); Method = "Overlaying"; }
                                    if (method == "screen") { Screen(tracking, image); Method = "Screening"; }

                                    if (index == 3) {
                                        cout << Method << " " << ifile << " and " << arg << " ..." << endl;
                                    }
                                    else {
                                        cout << " and " << method << "ing " << arg << " from previous step ..." << endl;
                                    }

                                    index++; // TODO: make sure the final index breaks out of the loop
                                }
                            }

                            index++; // move the pointer after each process

                        }
                        cout << "... saving output to " << ofile << "." << endl;
                        writeFile(header, tracking, ofile);

                    }
                    else {
                        cout << "Invalid method name." << endl;
                    }

                }
                else {
                    cout << "Invalid method name." << endl;
                }

            }
        }
    }
    return 0;
}

void writeFile(Header& header, Image& image, const string& filename) {
    ofstream file(filename, ios_base::binary);
    if (!file) {
        cerr << "Error opening file." << endl;
    }
    header.Serialize(file);
    for (unsigned char pixel : image.pixels) {
        file.write(reinterpret_cast<char*>(&pixel), sizeof(pixel));
    }
    file.close();
}

void readFile(Header& header, Image& image, const string& filename) {
    ifstream file(filename, ios_base::binary);

    header.readHeader(file);
    image.ReadPixels(file);

    file.close();
}

void Multiply(Image& tracking, const Image& arg) {
    for (int i = 0; i<tracking.pixels.size(); i++) {
        float layer1 = tracking.pixels[i]/255.0;
        float layer2 = arg.pixels[i]/255.0;
        float newValue = (layer1 * layer2) * 255.0;

        tracking.pixels[i] = newValue + 0.5f;
    }
}

void Subtract(Image& tracking, const Image& arg) {
    for (int i = 0; i < tracking.pixels.size(); i++ ) {
        int myPixel = tracking.pixels[i] - arg.pixels[i];
        if (myPixel < 0) {
            tracking.pixels[i] = 0;
        }
        else {
            tracking.pixels[i] = static_cast<unsigned char>(myPixel);
        }
    }
}

void Overlay(Image& tracking, const Image& arg) {

    for (int i = 0; i< tracking.pixels.size(); i++) {
        float pixel = tracking.pixels[i]/255.0;
        float pixel2 = arg.pixels[i]/255.0;
        float newpixel;

        if (pixel2 <= 0.5) {
            newpixel = (2*pixel*pixel2) * 255.0;
        }
        else {
            newpixel = (1 - (2 * (1-pixel) * (1-pixel2))) * 255.0;
        }
        tracking.pixels[i] = newpixel + 0.5f;
    }
}
void Screen(Image& image, const Image& image2) {

    for (int i = 0; i<image.pixels.size(); i++) {
        float pixel = image.pixels[i]/255.0;
        float pixel2 = image2.pixels[i]/255.0;
        float newPixel = (1 - ( (1-pixel) * (1-pixel2))) * 255.0;

        image.pixels[i] = newPixel + 0.5f;

    }

}
void Combine(Image& red, const Image& green, const Image& blue) {

    int b = 0, g = 1;
    for (int i = 0; i <red.pixels.size(); i++) {
        if (i == b) {
            red.pixels[i] = blue.pixels[i];
            b += 3;
        }
        if (i == g) {
            red.pixels[i] = green.pixels[i];
            g += 3;
        }
    }

}

void Flip(Image& tracking) {

    Image copy;
    for (unsigned char pixel : tracking.pixels) {
        copy.pixels.push_back(pixel);
    }
    for(int i = 0 ; i < tracking.pixels.size(); i+=3) {
        tracking.pixels[i] = copy.pixels[copy.pixels.size() - 3 - i];
        tracking.pixels[i + 1] = copy.pixels[copy.pixels.size() - 2 - i];
        tracking.pixels[i + 2] = copy.pixels[copy.pixels.size() - 1 - i];
    }

}

Image channelImage(const Image& image, const int value, const int channel) {
    Image newImage;
    int index = channel;

    for (int i = 0 ;i < image.pixels.size(); i++) {
        if (i == index) {
            newImage.pixels.push_back(value);
            index += 3;
        }
        else {
            newImage.pixels.push_back(0);
        }
    }
    return newImage;
}

void Scale(Image& image, int channel, const int& scale) {

    for (int i = 0; i<image.pixels.size(); i++) {
        int value = image.pixels[i];
        if (i == channel) {
            value *= scale;
            if (value > 255) {
                image.pixels[i] = 255;
            }
            else {
                image.pixels[i] = value;
            }
            channel += 3;
        }
    }

}

void onlygreen(Image& green) {

    vector <unsigned char> g_channel;
    for(int i = 1; i < green.pixels.size(); i+=3) {
        g_channel.push_back(green.pixels[i]);
    }
    for (int i = 0, j = 0; i < green.pixels.size(); i += 3, ++j) {
            green.pixels[i] = g_channel[j];
            green.pixels[i + 1] = g_channel[j];
            green.pixels[i + 2] = g_channel[j];

    }

}

void onlyblue(Image &blue) {

    vector <unsigned char> b_channel;
    for(int i = 0; i < blue.pixels.size(); i+=3) {
        b_channel.push_back(blue.pixels[i]);
    }
    for (int i = 0, j = 0; i < blue.pixels.size(); i += 3, ++j) {
            blue.pixels[i] = b_channel[j];
            blue.pixels[i + 1] = b_channel[j];
            blue.pixels[i + 2] = b_channel[j];
    }

}

void onlyred(Image &red) {

    vector<unsigned char> r_channel;
    for(int i = 2; i < red.pixels.size(); i+=3) {
        r_channel.push_back(red.pixels[i]);
    }
    for (int i = 0, j = 0; i < red.pixels.size(); i += 3, ++j) {
            red.pixels[i] = r_channel[j];
            red.pixels[i + 1] = r_channel[j];
            red.pixels[i + 2] = r_channel[j];
    }

}

void addred(Image& add, const string& value) {

    const int red = stoi(value);

    for (int i = 2; i < add.pixels.size(); i+= 3 ) {
        int myPixel = add.pixels[i] + red;
        if (myPixel > 255) {
            add.pixels[i] = 255;
        }
        else if (myPixel < 0) {
            add.pixels[i] = 0;
        }
        else {
            add.pixels[i] = myPixel;
        }
    }

}
void addgreen(Image& add, const string& value) {

    const int green = stoi(value);

    for (int i = 1; i < add.pixels.size(); i+= 3 ) {
        int myPixel = add.pixels[i] + green;
        if (myPixel > 255) {
            add.pixels[i] = 255;
        }
        else if (myPixel < 0) {
            add.pixels[i] = 0;
        }
        else {
            add.pixels[i] = myPixel;
        }
    }

}
void addblue(Image& add, const string& value) {

    const int blue = stoi(value);

    for (int i = 0; i < add.pixels.size(); i+= 3 ) {
        int myPixel = add.pixels[i] + blue;
        if (myPixel > 255) {
            add.pixels[i] = 255;
        }
        else if (myPixel < 0) {
            add.pixels[i] = 0;
        }
        else {
            add.pixels[i] = myPixel;
        }
    }

}

void scalered(Image& scale, const string& value) {
    const int mult = stoi(value);
    Scale(scale, 2, mult);
}

void scalegreen(Image& scale, const string& value) {
    const int mult = stoi(value);
    Scale(scale, 1, mult);
}

void scaleblue(Image& scale, const string& value) {
    const int mult = stoi(value);
    Scale(scale, 0, mult);
}

bool checkMethod(const string& method) {
    vector<string> methods = {"multiply", "subtract", "overlay", "screen", "flip",
    "addred", "addgreen", "addblue", "scalered", "scalegreen", "scaleblue", "onlyred",
    "onlygreen", "onlyblue", "combine"};
    for (string mode : methods) {
        if (mode == method) {
            return true;
        }
    }
    return false;;
}

bool trystoi(const string& str) {
    try {
        stoi(str);
    }
    catch (const invalid_argument&) {
        return false;
    }
    return true;
}