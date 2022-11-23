//
// Created by Jeppe on 17/11/2022.
//

#ifndef PRM_EXERCISE_GRAPH_H
#define PRM_EXERCISE_GRAPH_H


#include <opencv2/core/mat.hpp>
#include "Node.h"

class Graph
{
public:
    Graph(const cv::Mat &map, int n, int k, int scaleFactor = 4);
    bool obstacle(int x, int y);
    bool collision(cv::Point2i pointA, cv::Point2i pointB);
    void expand(Node *node);
    void visualize(bool save = false);
    void setGoalPos(int x, int y);
    void setStartPos(int x, int y);
    std::vector<Edge *> query(bool visualize = false, bool save = false);
    [[nodiscard]] cv::Point scaledPos(cv::Point pos) const;
    std::vector<Node *> nodes;
    std::vector<Edge *> edges;
    std::vector<Edge *> path;
    void dispose();
private:
    cv::Mat map;
    int scaleFactor;
    int n, k;
    Node *goal = nullptr, *start = nullptr;
    bool edgeExists(Edge edge);
    std::vector<Node *> sortedNearest(const Node &node);
    timeval begin, end;
};


#endif //PRM_EXERCISE_GRAPH_H
