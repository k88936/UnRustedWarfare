//
// Created by root on 11/28/24.
//
#include "PathFind.h"

#include <MapConfig.h>

PathFind::PathFind(float from_x, float from_y, float to_x, float to_y, movementType movement): movement_(movement)
{
    start_ = new PathNode(MapConfig::x_in_which(from_x), MapConfig::y_in_which(from_y), nullptr);
    end_ = new PathNode(MapConfig::x_in_which(to_x), MapConfig::y_in_which(to_y), nullptr);
    find_path(start_);
    for (const auto n : nodes_)
    {
        delete n;
    }
    delete start_;
    delete end_;
}


float PathFind::pass_look_up(int x, int y) const
{
    if (closed_set_.contains(PathNode::hash(x, y)))
    {
        return 0;
    }
    if (movement_ & MapConfig::get_tile_attribute(x,y)->type)
    {
        return MapConfig::get_tile_attribute(x,y)->pass_cost;
    }

    else
    {
        return 0;
    };
}

void PathFind::expand(int x, int y, PathNode* parent, float cost_multiply)
{
    const float cost = pass_look_up(x, y);
    if (cost == 0)return;
    const auto new_node = new PathNode(x, y, parent);
    closed_set_.insert(new_node->id);
    new_node->h = abs(end_->x - x) + abs(end_->y - y); //+0.001f*((end->x-x)*(end->x -x)+(end->y-y)*(end->y-y));
    new_node->g = parent->g + cost * cost_multiply;
    // path_node* grand=parent->parent;
    // if (grand!=nullptr&&grand->parent!=nullptr)
    // {
    // path_node* ggrand=grand->parent;
    //     new_node->g += 0.5f *std::abs((ggrand->x - grand->x) * (ggrand->y - y) - (ggrand->y - grand->y) * (ggrand->x - x));
    // }
    new_node->f = 1 * new_node->g + 2 * new_node->h;
    nodes_.push_back(new_node);
    open_set_.push(new_node);
}

bool PathFind::find_path(PathNode* cur)
{
    int cnt = 0;
    constexpr float addition = 1.414f;
    while (true)
    {
        if (cur->x == end_->x && cur->y == end_->y)
        {
            while (cur->parent != nullptr)
            {
                path.emplace_back(cur->x, cur->y);
                cur = cur->parent;
            }
            std::ranges::reverse(path);
            return true;
        }
        expand(cur->x - 1, cur->y - 1, cur, addition);
        expand(cur->x - 1, cur->y, cur, 1);
        expand(cur->x - 1, cur->y + 1, cur, addition);
        expand(cur->x, cur->y - 1, cur, 1);
        expand(cur->x, cur->y + 1, cur, 1);
        expand(cur->x + 1, cur->y - 1, cur, addition);
        expand(cur->x + 1, cur->y, cur, 1);
        expand(cur->x + 1, cur->y + 1, cur, addition);
        if (open_set_.empty())
        {
            return false;
        }
        cur = open_set_.top();
        open_set_.pop();
        // qDebug() << cnt << " : " << cur->x << ' ' << cur->y << " --> " << cur->f;
        cnt++;
        // if (cnt>1000)
        // {
        //     return false;
        // }
    }
}
