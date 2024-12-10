//
// Created by root on 11/30/24.
//

#ifndef FLOWFIELD_H
#define FLOWFIELD_H
#include <queue>
#include <QVector3D>
#include <set>
#include <vector>

#include "structures/movement_type.h"
#include "structures/path_node.h"


class Game;

class FlowField
{
public:
    FlowField(Game* game, float to_x, float to_y, movementType movement);
    QVector3D& get_vector(int x, int y);

private:
    std::vector<std::vector<QVector3D>> field_;
    float get_cost(Game*, int x, int y) const;
    void expand(Game* game, int x, int y, const PathNode* parent, float cost_multiply);
    void flow(Game* game, PathNode* end);
    std::priority_queue<PathNode*, std::vector<PathNode*>, PathNode::cmp> open_set_;
    std::set<int> closed_set_;

    std::vector<std::vector<PathNode*>> nodes_;
    movementType movement_;
};


#endif //FLOWFIELD_H
