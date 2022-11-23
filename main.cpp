#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "prm/Graph.h"
#include "prm/Edge.h"

cv::Mat read_map(const std::string& file) {
    cv::Mat map_image = cv::imread("../maps/" + file, cv::IMREAD_GRAYSCALE);
    cv::Mat map_binary(map_image.size(), CV_8UC1);
    cv::threshold(map_image, map_binary, 100, 255, cv::THRESH_BINARY);

    cv::imshow("Map", map_binary);
    return map_binary;
}

int main()
{
//    cv::Mat map = read_map("prg_ex1_map.png");
    cv::Mat map = read_map("map_01.png");

//    int n = 10;
//    int k = 3;
//    for (int i = 0; i < 5; ++i)
//    {
//        for (int j = 2; j < 3; ++j)
//        {
//            std::cout << "n: " << n << ", k: " << k << std::endl;
//            Graph myGraph = Graph(map, n, k);
//
//            myGraph.setStartPos(10, 10);
//            myGraph.setGoalPos(map.cols-69, map.rows-40);
//
//            myGraph.query(true, true);
//            k *= 2;
//            myGraph.dispose();
//        }
//        k = 3;
//        n *= 10;
//    }

    Graph myGraph = Graph(map, 1000, 16);

    myGraph.setStartPos(10, 10);
    myGraph.setGoalPos(map.cols-69, map.rows-40);

    myGraph.query(true, true);

    cv::waitKey(0);
    return 0;
}