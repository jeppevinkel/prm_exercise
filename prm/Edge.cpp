//
// Created by Jeppe on 17/11/2022.
//

#include "Edge.h"

Edge::Edge(Node *left, Node *right) : distance(left->distance(right)), left(left), right(right) {
    left->neighbors.push_back(this);
//    right->neighbors.push_back(this);
}

bool Edge::operator==(const Edge &rhs) const
{
    return ((this->left == rhs.left && this->right == rhs.right) || (this->left == rhs.right && this->right == rhs.left));
}
