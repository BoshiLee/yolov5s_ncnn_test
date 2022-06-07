//
// Created by boshi on 2022/6/6.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "headers/net.h"

int main()
{
    cv::Mat img = cv::imread("face.jpg", cv::IMREAD_GRAYSCALE);
    int w = img.cols;
    int h = img.rows;

    // subtract 128, norm to -1 ~ 1
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_GRAY, w, h, 60, 60);
    float mean[1] = { 128.f };
    float norm[1] = { 1/128.f };
    in.substract_mean_normalize(mean, norm);

    ncnn::Net net;
    net.load_param("models/v5s-p6_wider_face_sim-opt/model.param");
    net.load_model("models/v5s-p6_wider_face_sim-opt/model.bin");

    ncnn::Extractor ex = net.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);

    ex.input("images", in);

    ncnn::Mat feat;
    ex.extract("output", feat);
    printf("x = %d\n", feat);
    return 0;
}