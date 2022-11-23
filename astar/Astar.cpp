#include "Astar.h"
#include "../prm/edge.h"

AStar::AStar()
{
}

std::vector<Edge*> AStar::shortestPath(Node *initNode,Node *goalNode)
{
    if(open.size())
        open.erase(open.begin(),open.end());
    if(closed.size())
        closed.erase(closed.begin(),closed.end());
    AStarInfo n;
    n.node = initNode;
    n.came_from = NULL;
    n.c_percurso = 0;
    n.c_total = n.c_percurso + n.node->distance(goalNode);
    open.push_back(&n);

    while(open.size())
    {
        //std::cout << "A* Open: " << open.size() << std::endl;
        //std::cout << "A* Closed: " << closed.size() << std::endl;
        AStarInfo *current = open.at(0);
        for(int i=1;i<open.size();i++)
        {
            if(current->c_total > open.at(i)->c_total)
                current = open.at(i);
        }
        //std::cout << "A* Current: " << current->node->mId << " " << initNode << current->node << goalNode << current->node->neighbors.size() << std::endl;

        if(current->node == goalNode)
        {
//            std::cout << "A*: Finalizando..." << std::endl;
            std::vector<Edge*> ret;
            while(current->came_from)
            {
                ret.insert(ret.begin(), new Edge(current->came_from->node, current->node));
                current = current->came_from;
            }
            //std::cout << "A*: Finalizado!" << std::endl << std::endl;
            return ret;
        }
        open.erase(std::remove(open.begin(),open.end(),current),open.end());
        closed.push_back(current);

        for(int i =0;i<current->node->neighbors.size();i++)
        {
            double tentativa = current->c_percurso + current->node->neighbors.at(i)->distance;
            //qDebug() << "A* Neighbor: " << current->node->neighbors.at(i)->mNodeR->mId;

            AStarInfo *infoAStar = getInfoClosedNode(current->node->neighbors.at(i)->right);
            if (( infoAStar != NULL) && tentativa >= infoAStar->c_percurso)
            {
                continue;
            }

            infoAStar = getInfoNode(current->node->neighbors.at(i)->right);


            if( (getInfoOpenNode(current->node->neighbors.at(i)->right) == NULL) || (tentativa < infoAStar->c_percurso) )
            {
                if(infoAStar == NULL)
                    infoAStar = new AStarInfo();
                infoAStar->came_from = current;
                infoAStar->c_percurso = tentativa;
                infoAStar->c_total = infoAStar->c_percurso + current->node->neighbors.at(i)->right->distance(goalNode);
                infoAStar->node = current->node->neighbors.at(i)->right;

                if(getInfoOpenNode(current->node->neighbors.at(i)->right) == NULL)
                    open.push_back(infoAStar);
            }
        }
    }
//    qDebug() << "A*: Finalizado sem resposta!";
    return std::vector<Edge *> ();
}

AStarInfo* AStar::getInfoClosedNode(Node *node)
{
    return getInfoNode(closed,node);
}

AStarInfo* AStar::getInfoOpenNode(Node *node)
{
    return getInfoNode(open,node);
}

AStarInfo* AStar::getInfoNode(Node *node)
{
    AStarInfo *info;
    info = getInfoOpenNode(node);
    if(info == NULL)
        info = getInfoClosedNode(node);
    return info;
}

AStarInfo* AStar::getInfoNode(std::vector<AStarInfo *> vector, Node *node)
{
    for(int i=0;i<vector.size();i++)
    {
        if(vector.at(i)->node == node)
        {
            return vector.at(i);
        }
    }
    return NULL;
}
