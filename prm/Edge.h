//
// Created by Jeppe on 17/11/2022.
//

#ifndef PRM_EXERCISE_EDGE_H
#define PRM_EXERCISE_EDGE_H


#include "Node.h"

class Edge
{
public:
    Edge(Node *left, Node *right);
    Node *left;
    Node *right;
    const double distance;

    bool operator==(Edge const& rhs) const;
};


#endif //PRM_EXERCISE_EDGE_H
