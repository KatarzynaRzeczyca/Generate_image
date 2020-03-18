#include "Header.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    string image_file;
    int elements = 50;
    if (argc == 3) {
        image_file = argv[1];
        elements = atoi(argv[2]);
    }
    else if (argc == 2) {
        image_file = argv[1];
    }
    else {
        cout << "ARGERROR" << endl;
        return -1;
        }
    try {
        Mat image;
        image = imread(argv[1], IMREAD_COLOR);
        if (!image.data) {
            throw;
        }
    }
    catch (Exception &e){
        cout << "FILE_ERROR" << endl;
        return -1;
        }
    GenerateImage g = GenerateImage(argv[1], elements);
    try {
        g.main_action();
    }
    catch (Exception & e) {
        g.save_result();
        return -1;
    }
    return 0;
    //GenerateImage im = GenerateImage(argv[1]);

    //Mat image;
    //image = imread(argv[1], IMREAD_COLOR); // Read the file

    //if (!image.data) // Check for invalid input
    //{
    //    cout << "Could not open or find the image" << std::endl;
    //    return -1;
    //}

    //namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
    //imshow("Display window", image); // Show our image inside it.

    //waitKey(0); // Wait for a keystroke in the window
    //return 0;
}