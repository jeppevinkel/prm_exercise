//
// Created by Jeppe on 17/11/2022.
//

#include <cmath>
#include <iostream>
#include "Node.h"

Node::Node() : id(std::rand()%1000) {}
Node::Node(int id) : id(id) {}

Node::Node(int id, int posX, int posY) : id(id)
{
    this->pos = cv::Point(posX, posY);
}

Node::Node(int posX, int posY) : id(std::rand()%1000)
{
    this->pos = cv::Point(posX, posY);
}

double Node::distance(const Node *node) const
{
    return sqrt(pow(this->pos.x - node->pos.x, 2) + pow(this->pos.y - node->pos.y, 2));
}

void Node::print() const
{
    std::cout << "(" << this->id << ") [" << this->pos.x << ", " << this->pos.y << "]" << std::endl;
}

std::string Node::toString() const
{
    return "(" + std::to_string(this->id) + ") [" + std::to_string(this->pos.x) + ", " + std::to_string(this->pos.y) + "]";
}

bool Node::operator==(const Node &rhs) const
{
    return this->id == rhs.id && this->pos == rhs.pos;
}
