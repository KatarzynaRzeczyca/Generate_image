#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

class Triangle {
public:
    int r;
    int g;
    int b;
    int points[6]; //x1, y1, x2, y2, x3, y3

    Triangle();
};
void copy(Triangle& in, Triangle& out);

class GenerateImage {
public:
    cv::Mat origin_img;
    int height;
    int width;
    float d_max;
    int num_of_elements;

    int mutated_shape = 0;
    cv::Mat image_best;
    cv::Mat image_test;
    float mut = 1;
    std::vector<Triangle> best_elements;
    std::vector<Triangle> test_elements;

    GenerateImage(std::string image_file, int num_of_el = 50);
    Triangle generate_triangle();
    void generate_image(std::vector<Triangle>& elements, cv::Mat& img);
    std::vector<Triangle> generate_random_elements();
    float compare_images(cv::Mat& image_origin, cv::Mat& image_copy);
    int clamp(int arg, int min, int max);
    int mutate(float fitness);
    void main_action();
    void save_result();
};