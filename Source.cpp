#include "Header.h"

using namespace cv;
using namespace std;

Triangle::Triangle() {
        r = 0;
        g = 0;
        b = 0;
        for (int i = 0; i < 6; i++) {
            points[i] = 0;
        }

        };

void copy(Triangle& in, Triangle& out) {
    out.r = in.r;
    out.g = in.g;
    out.b = in.b;
    for (int i = 0; i < 6; i++) {
        out.points[i] = in.points[i];
    }
}

GenerateImage::GenerateImage(string image_file, int num_of_el) {
            origin_img = cv::imread(image_file, -1);
            //origin_img = imread(image_file, IMREAD_COLOR);
            height = origin_img.size().height;
            width = origin_img.size().width;
            d_max = (float)(height*width*sqrt(3));
            srand(time(NULL));
            num_of_elements = num_of_el;
            /*Mat image_best(height, width, CV_16UC3, Scalar(0, 0, 0));
            Mat image_test(height, width, CV_16UC3, Scalar(0, 0, 0));*/
            Mat image_best(height, width, origin_img.type(), Scalar(0, 0, 0));
            Mat image_test(height, width, origin_img.type(), Scalar(0, 0, 0));
        }
Triangle GenerateImage::generate_triangle()
{
    int w = width;
    int h = height;
    Triangle t;
    t.points[0] = rand() % w;
    t.points[1] = rand() % h;
    t.points[2] = rand() % w;
    t.points[3] = rand() % h;
    t.points[4] = rand() % w;
    t.points[5] = rand() % h;
    t.r = rand() % 256;
    t.g = rand() % 256;
    t.b = rand() % 256;
    return t;
};
void GenerateImage::generate_image(vector<Triangle>& elements, Mat& img_out) {
    Mat img(height, width, origin_img.type(), Scalar(0, 0, 0));
    Triangle triangle;
    int lineType = LINE_8;
    for (int i = 0; i < elements.size(); i++) {
        triangle = elements[i];
        Point tr[1][3];
        tr[0][0] = Point(triangle.points[0], triangle.points[1]);
        tr[0][1] = Point(triangle.points[2], triangle.points[3]);
        tr[0][2] = Point(triangle.points[4], triangle.points[5]);
        const Point* ppt[1] = { tr[0] };
        int npt[] = { 3 };
        fillPoly(img, ppt, npt, 1, Scalar(triangle.b, triangle.g, triangle.r), lineType);
    }
    img_out = img.clone();
}
vector<Triangle> GenerateImage::generate_random_elements() {
    vector<Triangle> list_of_elements;
    for (int i = 0; i < num_of_elements; i++) {
        list_of_elements.push_back(generate_triangle());
    }
    return list_of_elements;
}
float GenerateImage::compare_images(Mat& image_origin, Mat& image_copy) {
    int d = 0;
    Mat sub_array(image_origin.rows, image_origin.cols, image_origin.type());
    Mat sub1;
    Mat sub2;
    /*image_origin.convertTo(sub1, CV_16U);
    image_origin.convertTo(sub2, CV_16U);*/
    image_origin.convertTo(sub1, image_origin.type());
    image_copy.convertTo(sub2, image_copy.type());
    sub_array = sub1 - sub2;
    Mat pow_array(image_origin.rows, image_origin.cols, image_origin.type());
    pow(sub_array, 2, pow_array);
    Mat *bgr = new Mat[pow_array.channels()];
    split(pow_array, bgr);
    Mat sum_array(image_origin.rows, image_origin.cols, image_origin.type());
    if (pow_array.channels() >= 2) {
        add(bgr[0], bgr[1], sum_array);
        for (int i = 2; i + 1 < pow_array.channels(); i++) {
            add(bgr[i], sum_array, sum_array);
        }
    }
    //Mat sum_array2;
    //add(bgr[2], sum_array, sum_array2);
    delete[] bgr;
    Mat result_array(image_origin.rows, image_origin.cols, CV_64F);
    Mat sum3;
    sum_array.convertTo(sum3, CV_64F);
    sqrt(sum3, result_array);
    Scalar sum_elems_scal = sum(result_array);
    double sum_elems = sum_elems_scal[0] + sum_elems_scal[1] + sum_elems_scal[2] + sum_elems_scal[3];
    float sum_elems2 = (float)(sum_elems);
    return sum_elems2;
}
int GenerateImage::clamp(int arg, int min, int max) {
    if (arg > max)
        return max;
    else if (arg < min)
        return min;
    return arg;
}
int GenerateImage::mutate(float fitness) {
    int h = height;
    int w = width;
    int mutated_shape = rand() % num_of_elements;
    float roulette = 2.8f * ((float)rand()) / (float)RAND_MAX;
    int col_mut = int(mut * 25);
    int width_mut = int(mut * 0.1f * width);
    int height_mut = int(mut * 0.1f * height);
    float drastic;
    if (fitness < 94.0f)
        drastic = 2.0f * ((float)rand()) / (float)RAND_MAX;
    else
        drastic = 0.0f;
    if (roulette < 1.0f) {
        if (roulette < 0.33f) {
            if (drastic < 1) {
                test_elements[mutated_shape].r += rand() % (2 * col_mut) - col_mut;
                test_elements[mutated_shape].r = clamp(test_elements[mutated_shape].r, 0, 255);
            }
            else {
                test_elements[mutated_shape].r = rand() % 256;
            }
        }
        else if (roulette < 0.66f) {
            if (drastic < 1) {
                test_elements[mutated_shape].g += rand() % (2 * col_mut) - col_mut;
                test_elements[mutated_shape].g = clamp(test_elements[mutated_shape].g, 0, 255);
            }
            else {
                test_elements[mutated_shape].g = rand() % 256;
            }
        }
        else {
            if (drastic < 1) {
                test_elements[mutated_shape].b += rand() % (2 * col_mut) - col_mut;
                test_elements[mutated_shape].b = clamp(test_elements[mutated_shape].b, 0, 255);
            }
            else {
                test_elements[mutated_shape].b = rand() % 256;
            }
        }
    }
    else if (roulette < 2.0f) {
        int p = 2 * (rand() % 3);
        if (roulette < 1.5f) {
            if (drastic < 1) {
                test_elements[mutated_shape].points[p] += (rand() % (2 * width_mut)) - width_mut;
                test_elements[mutated_shape].points[p] = clamp(test_elements[mutated_shape].points[p], 0, width - 1);
            }
            else {
                test_elements[mutated_shape].points[p] = (width - 1) * (int)(rand() / RAND_MAX);
            }
        }
        else {
            if (drastic < 1) {
                test_elements[mutated_shape].points[p + 1] += (rand() % (2 * height_mut)) - height_mut;
                test_elements[mutated_shape].points[p + 1] = clamp(test_elements[mutated_shape].points[p + 1], 0, height - 1);
            }
            else {
                test_elements[mutated_shape].points[p + 1] = (height - 1) * (int)(rand() / RAND_MAX);
            }
        }
    }
    else {
        int destination = rand() % num_of_elements;
        Triangle temp_shape = test_elements[mutated_shape];
        test_elements[mutated_shape] = test_elements[destination];
        test_elements[destination] = temp_shape;
        return destination;
    }
    return -1;
}
void GenerateImage::main_action() {
    best_elements = generate_random_elements();
    Mat img(height, width, origin_img.type(), Scalar(0, 0, 0));
    generate_image(best_elements, image_best);
    image_test = image_best.clone();
    test_elements = best_elements;
    float lowestdiff = (float)INT_MAX;
    int generation = 0;
    int beststep = 0;
    float d = 0.0;
    float fitness = 0.0;
    while (fitness < 95.0f) {
        int other_mutated = mutate(fitness);
        generate_image(test_elements, image_test);
        d = compare_images(origin_img, image_test);
        if (d < lowestdiff) {
            copy(test_elements[int(mutated_shape)], best_elements[int(mutated_shape)]);
            if (other_mutated >= 0) {
                copy(test_elements[int(other_mutated)], best_elements[int(other_mutated)]);
            }
            lowestdiff = d;
            beststep += 1;
            generate_image(best_elements, image_best);
            imshow("wynik", image_best);
            waitKey(10);
            cout << generation << " | " << fitness << " | " << beststep << endl;
        }
        else {
            copy(best_elements[int(mutated_shape)], test_elements[int(mutated_shape)]);
            if (other_mutated >= 0) {
                copy(best_elements[int(other_mutated)], test_elements[int(other_mutated)]);
            }
        }
        fitness = float(100 * (d_max - lowestdiff) / d_max);
        if (fitness > 91.0f and mut > 0.6f) {
            mut = 0.8f;
        }
        generation += 1;
    }
    generate_image(best_elements, image_best);
    imshow("wynik", image_best);
    waitKey(500);
}
void GenerateImage::save_result() {
    generate_image(best_elements, image_best);
    imwrite("wynik.jpg", image_best);
}
