#ifndef ASTAR_H
#define ASTAR_H

#include "../prm/graph.h"

typedef struct __AStarInfo
{
    struct __AStarInfo *came_from;
    Node *node;
    double c_percurso, c_total;
} AStarInfo;

class AStar
{
public:
    AStar();
    std::vector<Edge*> shortestPath(Node *initNode,Node *goalNode);

protected:
    AStarInfo* getInfoClosedNode(Node *node);
    AStarInfo* getInfoOpenNode(Node *node);
    AStarInfo* getInfoNode(Node *node);
    AStarInfo* getInfoNode(std::vector<AStarInfo*> vector, Node *node);
    std::vector<AStarInfo*> closed;
    std::vector<AStarInfo*> open;
};

#endif // ASTAR_H
