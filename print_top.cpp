// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2017 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "headers/net.h"
#include <iostream>
#include <algorithm>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdio.h>
#include <vector>

static int detect_yolo(const cv::Mat& bgr, std::vector<float>& cls_scores)
{
    int w = bgr.cols;
    int h = bgr.rows;

    int target_width = 640;// target resized width
    int target_height = 640;// target resized height

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_GRAY, w, h, target_width, target_height);
    float mean[1] = { 128.f };
    float norm[1] = { 1/128.f };
    in.substract_mean_normalize(mean, norm);

    ncnn::Net yolo;
    yolo.load_param("models/v5s-p6_wider_face_sim-opt/model.param");
    yolo.load_model("models/v5s-p6_wider_face_sim-opt/model.bin");

    ncnn::Extractor ex = yolo.create_extractor();
    ex.set_light_mode(true);
    ex.set_num_threads(4);
    ex.input("images", in);

    ncnn::Mat out;
    ex.extract("output", out);
    std::cout << out.w << "\n";
    cls_scores.resize(out.w);
    for (int j = 0; j < out.w; j++)
    {
        cls_scores[j] = out[j];
    }

    return 0;
}

static int print_topk(const std::vector<float>& cls_scores, int topk)
{
    // partial sort topk with index
    int size = cls_scores.size();
    std::vector<std::pair<float, int> > vec;
    vec.resize(size);

    for (int i = 0; i < size; i++)
    {
        vec[i] = std::make_pair(cls_scores[i], i);

    }

//    std::partial_sort(vec.begin(), vec.begin() + topk, vec.end(),
//                      std::greater<std::pair<float, int> >());
//

    // print topk and score
//    for (int i = 0; i < topk; i++)
//    {
//        float score = vec[i].first;
//        int index = vec[i].second;
//        fprintf(stderr, "%d = %f\n", index, score);
//    }

    return 0;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }

    const char* imagepath = argv[1];

    cv::Mat m = cv::imread(imagepath, 1);
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }

    std::vector<float> cls_scores;
    detect_yolo(m, cls_scores);

    print_topk(cls_scores, 3);

    return 0;
}
