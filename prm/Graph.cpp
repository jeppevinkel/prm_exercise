//
// Created by Jeppe on 17/11/2022.
//

#include "Graph.h"
#include "Edge.h"
#include "../astar/Astar.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>
#include <ctime>

Graph::Graph(const cv::Mat &map, int n, int k, int scaleFactor) : n(n), k(k), scaleFactor(scaleFactor)
{
    this->map = map;
    std::default_random_engine generator(time(0));
    std::uniform_int_distribution<int> randX(0,this->map.cols);
    std::uniform_int_distribution<int> randY(0,this->map.rows);
    mingw_gettimeofday(&begin, nullptr);
    while (this->nodes.size() < n)
    {
        Node *node;
        do
        {
            node = new Node(this->nodes.size(), randX(generator), randY(generator));
//            node->print();
        } while (this->obstacle(node->pos.x, node->pos.y));
        this->nodes.push_back(node);
    }

    for (auto &node: this->nodes)
    {
//        std::cout << node->toString() << std::endl;
        std::vector<Node *> nodeList;
        for (auto &_node : this->nodes)
        {
            nodeList.push_back(_node);
        }
        std::vector<Node *> kNeighbors;
        while (kNeighbors.size() < k)
        {
            auto shortestDist = DBL_MAX;
            int closestNode;
            for (int i = 0; i < nodeList.size(); i++)
            {
                if (node == nodeList[i]) continue;
                double currentDistance = node->distance(nodeList[i]);
                if (currentDistance < shortestDist)
                {
                    shortestDist = currentDistance;
                    closestNode = i;
                }
            }
            kNeighbors.push_back(nodeList[closestNode]);
            nodeList.erase(nodeList.begin() + closestNode);
        }

        for (auto & kNeighbor : kNeighbors)
        {
            if (!collision(node->pos, (*kNeighbor).pos)) {
                Edge *edge = new Edge(node, kNeighbor);
                if (!edgeExists(*edge)) {
//                    std::cout << edge->left.toString() << "  <--->  " << edge->right.toString() << std::endl;
                    this->edges.push_back(edge);
                }
            }
        }
    }

    bool didExpand = false;
    do
    {
        didExpand = false;
        for (const auto &node : this->nodes)
        {
//            std::cout << "Neigh: " << node->neighbors.size() << std::endl;
            if (node->neighbors.size() < 2) {
                didExpand = true;
                expand(node);
//                std::cout << "NewNeigh: " << node->neighbors.size() << std::endl;
            }
        }
    } while (didExpand);
}

bool Graph::obstacle(int x, int y)
{
    if (x < 0 || y < 0 || x >= this->map.cols || y >= this->map.rows) return true;

    return map.at<uint8_t>(y, x) == 0;
}

void Graph::visualize(bool save)
{
    cv::Mat map_colorized;
    cv::cvtColor(this->map, map_colorized, cv::COLOR_GRAY2BGR);

    cv::resize(map_colorized, map_colorized, cv::Size2d(map_colorized.cols * this->scaleFactor, map_colorized.rows * this->scaleFactor), 0, 0,
               cv::INTER_NEAREST);

    for (auto &edge : this->edges)
    {
//        std::cout << edge->left.toString() << "  <- [" + std::to_string(edge->distance) + "] ->  " << edge->right.toString() << std::endl;
        cv::line(map_colorized, this->scaledPos(edge->left->pos), this->scaledPos(edge->right->pos), cv::Scalar(255, 255, 0), 1);
    }

    for (const auto &node: this->nodes)
    {
        if (node == this->start) {
            cv::circle(map_colorized, this->scaledPos(this->start->pos), 3, cv::Scalar(255, 0, 0), -1);
        } else if (node == this->goal) {
            cv::circle(map_colorized, this->scaledPos(this->goal->pos), 3, cv::Scalar(0, 255, 0), -1);
        } else {
            cv::circle(map_colorized, this->scaledPos(node->pos), 1, cv::Scalar(255, 0, 255), -1);
        }
    }

    if (!this->path.empty()) {
        for (const auto $_edge : this->path)
        {
            cv::line(map_colorized, this->scaledPos($_edge->left->pos), this->scaledPos($_edge->right->pos), cv::Scalar(0, 0, 255), 2);
        }
    }

//    if (this->start) {
//        cv::circle(map_colorized, this->scaledPos(this->start->pos), 3, cv::Scalar(255, 0, 0), -1);
//    }
//    if (this->goal) {
//        cv::circle(map_colorized, this->scaledPos(this->goal->pos), 3, cv::Scalar(0, 255, 0), -1);
//    }

    cv::putText(map_colorized,
                "Time elapsed: " + std::to_string(((double)(end.tv_usec-begin.tv_usec))/1000000 + (double)(end.tv_sec-begin.tv_sec)) + " sec",
                cv::Point(10, map_colorized.rows - 10),
                cv::FONT_HERSHEY_COMPLEX,
                1,
                cv::Scalar(0, 0, 0));

    if (save) {
        std::string filename = "../out/result_n" + std::to_string(this->n) + "_k" + std::to_string(this->k) + ".png";
        std::cout << "Saving file as `" << filename << "`" << std::endl;
        cv::imwrite(filename, map_colorized);
    }
    cv::imshow("Graph Visualizer N=" + std::to_string(this->n) + ", K=" + std::to_string(this->k), map_colorized);
}

bool Graph::collision(cv::Point2i pointA, cv::Point2i pointB)
{
    if (pointA.x < 0 || pointA.y < 0 || pointB.x < 0 || pointB.y < 0)
    {
        return true;
    }

    cv::LineIterator lineIterator(this->map, pointA, pointB);

    for (int i = 0; i < lineIterator.count; i++)
    {
        if (map.at<uint8_t>(lineIterator.pos()) == 0)
        {
            return true;
        }
        lineIterator++;
    }

    return false;
}

cv::Point Graph::scaledPos(cv::Point pos) const
{
    return {(pos.x) * (this->map.cols * this->scaleFactor) / (this->map.cols),
            (pos.y) * (this->map.rows * this->scaleFactor) / (this->map.rows)};
}

bool Graph::edgeExists(Edge edge)
{
    for (auto &e: this->edges)
        if (&edge == e) return true;

    return false;
}

void Graph::setGoalPos(int x, int y)
{
    if (!obstacle(x, y)) {
        delete goal;

        goal = new Node(x, y);
    }
}

void Graph::setStartPos(int x, int y)
{
    if (!obstacle(x, y)) {
        delete start;

        start = new Node(x, y);
    }
}

std::vector<Edge *> Graph::query(bool visualize, bool save)
{
    this->nodes.push_back(start);
    this->nodes.push_back(goal);

    std::vector<Node *> kNeighbors = this->sortedNearest(*start);
    for (auto & kNeighbor : kNeighbors)
    {
        if (!collision(start->pos, (*kNeighbor).pos)) {
            Edge *edge = new Edge(start, kNeighbor);
            this->edges.push_back(edge);
            break;
        }
    }

    kNeighbors = this->sortedNearest(*goal);
    for (auto & kNeighbor : kNeighbors)
    {
        if (!collision(goal->pos, (*kNeighbor).pos)) {
            Edge *edge = new Edge(kNeighbor, goal);
            this->edges.push_back(edge);
            break;
        }
    }

    AStar aStar;
    path = aStar.shortestPath(start, goal);
    mingw_gettimeofday(&end, nullptr);
    std::cout << path.size() << std::endl;

//    if(path.empty())
//    {
//        delete start;
//        delete goal;
//        start = goal = nullptr;
//    }

    if (visualize)
        this->visualize(save);

    this->edges.pop_back();
    this->edges.pop_back();
    this->nodes.pop_back();
    this->nodes.pop_back();

    return path;
}

std::vector<Node *> Graph::sortedNearest(const Node &node)
{
    std::vector<Node *> nodeList;
    for (auto &_node : this->nodes)
    {
        nodeList.push_back(_node);
    }
    std::vector<Node *> kNeighbors;
    while (kNeighbors.size() < k)
    {
        auto shortestDist = DBL_MAX;
        int closestNode;
        for (int i = 0; i < nodeList.size(); i++)
        {
            if (node == *nodeList[i]) continue;
            double currentDistance = node.distance(nodeList[i]);
            if (currentDistance < shortestDist)
            {
                shortestDist = currentDistance;
                closestNode = i;
            }
        }
        kNeighbors.push_back(nodeList[closestNode]);
        nodeList.erase(nodeList.begin() + closestNode);
    }

    return kNeighbors;
}

void Graph::dispose()
{
    for (auto & edge : this->edges)
    {
        delete edge;
    }
    for (auto & node : this->nodes)
    {
        delete node;
    }
    for (auto & edge : this->path)
    {
        delete edge;
    }
    delete start;
    delete goal;
    start = goal = nullptr;
    this->nodes.clear();
    this->edges.clear();
    this->path.clear();
}

void Graph::expand(Node *node)
{
    cv::Point pos(node->pos);
    cv::Point newPos = pos;
    int dir = rand() % 8;
    while (!this->obstacle(newPos.x, newPos.y))
    {
        pos = newPos;
        switch (dir)
        {
            case 0:
                newPos = pos + cv::Point(-1, -1);
                break;
            case 1:
                newPos = pos + cv::Point(0, -1);
                break;
            case 2:
                newPos = pos + cv::Point(1, -1);
                break;
            case 3:
                newPos = pos + cv::Point(-1, 0);
                break;
            case 4:
                newPos = pos + cv::Point(1, 0);
                break;
            case 5:
                newPos = pos + cv::Point(-1, 1);
                break;
            case 6:
                newPos = pos + cv::Point(0, 1);
                break;
            case 7:
                newPos = pos + cv::Point(1, 1);
                break;
            default:
                std::cout << "This is all wrong" << std::endl;
                break;
        }
    }
    Node *myNode = new Node(this->nodes.size(), pos.x, pos.y);
    this->nodes.push_back(myNode);
    this->edges.push_back(new Edge(node, myNode));

    std::vector<Node *> kNeighbors = this->sortedNearest(*myNode);

    for (auto & kNeighbor : kNeighbors)
    {
        if (!collision(myNode->pos, (*kNeighbor).pos)) {
            Edge *edge = new Edge(myNode, kNeighbor);
            if (!edgeExists(*edge)) {
//                    std::cout << edge->left.toString() << "  <--->  " << edge->right.toString() << std::endl;
                this->edges.push_back(edge);
            }
        }
    }
}
