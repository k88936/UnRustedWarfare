//
// Created by root on 11/30/24.
//

#ifndef PATH_NODE_H
#define PATH_NODE_H

struct PathNode
{
    int x;
    int y;
    int id;
    float f = 0;
    float g = 0; //start to current
    float h = 0; //current to end
    PathNode* parent = nullptr;

    static int hash(int x, int y)
    {
        return x * 5211324 + y;
    }

    PathNode( int x, int y, PathNode* parent) : x(x), y(y), parent(parent)
    {
        id = hash(x, y);
    }

    struct cmp
    {
        bool operator()(const PathNode* a, const PathNode* b) const
        {
            return a->f > b->f;
        }
    };
};
#endif //PATH_NODE_H
