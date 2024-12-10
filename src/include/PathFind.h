// //
// // Created by root on 11/28/24.
// //
//
// #ifndef PATHFIND_H
// #define PATHFIND_H
// #include <queue>
// #include <set>
//
// #include "structures/movement_type.h"
// #include "structures/path_node.h"
//
// class PathFind
// {
// public:
//     PathFind(int from_x, int from_y, int to_x, int to_y, movementType movement);
//     std::vector<std::pair<int, int>> path;
//
// private:
//
//     std::priority_queue<PathNode*, std::vector<PathNode*>, PathNode::cmp> open_set_;
//     std::set<int> closed_set_;
//     std::vector<PathNode*> nodes_;
//     PathNode* start_;
//     PathNode* end_;
//     movementType movement_;
//     PathFind(float from_x, float from_y, float to_x, float to_y, movementType movement);
//     float pass_look_up(int x, int y) const;
//     void expand(int x, int y, PathNode* parent, float cost);
//     bool find_path(PathNode* cur);
// };
//
//
// #endif //PATHFIND_H
