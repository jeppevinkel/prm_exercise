//
// Created by Jeppe on 17/11/2022.
//

#ifndef PRM_EXERCISE_NODE_H
#define PRM_EXERCISE_NODE_H


#include <vector>
#include <opencv2/core/types.hpp>

class Edge;

class Node
{
public:
    Node();
    explicit Node(int id);
    Node(int posX, int posY);
    Node(int id, int posX, int posY);
    double distance(const Node* node) const;
    std::string toString() const;
    void print() const;
    std::vector<Edge *> neighbors;
    int id;
    cv::Point pos;

    bool operator==(Node const& rhs) const;
};


#endif //PRM_EXERCISE_NODE_H
