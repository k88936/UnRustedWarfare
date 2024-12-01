//
// Created by root on 11/30/24.
//

#include "FlowField.h"

#include <float.h>

FlowField::FlowField(const float to_x, const float to_y, const movementType movement): movement_(movement)
{
    int tile_x = MapConfig::x_in_which(to_x);
    int tile_y = MapConfig::y_in_which(to_y);

    field_.resize(MapConfig::world_width + 2);
    for (int i = -1; i < MapConfig::world_width + 1; ++i)
    {
        field_[i + 1].resize(MapConfig::world_height + 2);
    }
    if (get_cost(tile_x, tile_y) == 0)
    {
        return;
    }
    nodes_.resize(MapConfig::world_width + 2);
    for (int i = -1; i < MapConfig::world_width + 1; ++i)
    {
        nodes_[i + 1].resize(MapConfig::world_height + 2);
        for (int j = -1; j < MapConfig::world_height + 1; ++j)
        {
            nodes_[i + 1][j + 1] = new PathNode(i, j, nullptr);
            nodes_[i + 1][j + 1]->f = 5211324;
        }
    }

    const auto end = nodes_[tile_x + 1][tile_y + 1];
    end->f = 0;
    flow(end);

    for (int i = 0; i < MapConfig::world_width; ++i)
    {
        for (int j = 0; j < MapConfig::world_height; ++j)
        {
            float x = 0;
            float y = 0;

            x += nodes_[i][j]->f - nodes_[i + 1][j + 1]->f;;
            y += nodes_[i][j]->f - nodes_[i + 1][j + 1]->f;;

            x += nodes_[i][j + 1]->f - nodes_[i + 1][j + 1]->f;;

            x += nodes_[i][j + 2]->f - nodes_[i + 1][j + 1]->f;;
            y -= nodes_[i][j + 2]->f - nodes_[i + 1][j + 1]->f;;


            y += nodes_[i + 1][j]->f - nodes_[i + 1][j + 1]->f;;

            y -= nodes_[i + 1][j + 2]->f - nodes_[i + 1][j + 1]->f;;


            x -= nodes_[i + 2][j]->f - nodes_[i + 1][j + 1]->f;;
            y += nodes_[i + 2][j]->f - nodes_[i + 1][j + 1]->f;;

            x -= nodes_[i + 2][j + 1]->f - nodes_[i + 1][j + 1]->f;;

            x -= nodes_[i + 2][j + 2]->f - nodes_[i + 1][j + 1]->f;;
            y -= nodes_[i + 2][j + 2]->f - nodes_[i + 1][j + 1]->f;;

            get_vector(i, j).setX(x);
            get_vector(i, j).setY(y);
            get_vector(i, j).normalize();
        }
    }

    for (const auto n : nodes_)
    {
        for (const auto path_node : n)
        {
            delete path_node;
        }
    }
}

QVector3D& FlowField::get_vector(int x, int y)
{
    return field_.at(x + 1).at(y + 1);
}


float FlowField::get_cost(const int x, const int y) const
{
    if (closed_set_.contains(PathNode::hash(x, y)))
    {
        return 0;
    }
    if (movement_ & MapConfig::get_tile_attribute(x, y)->type)
    {
        return MapConfig::get_tile_attribute(x, y)->pass_cost;
    }

    return 0;
}

void FlowField::expand(int x, int y, const PathNode* parent, float cost_multiply)
{
    const float cost = get_cost(x, y);
    if (cost == 0)return;
    const auto new_node = nodes_[x + 1][y + 1];
    new_node->f = parent->f + cost * cost_multiply;
    open_set_.push(new_node);
    closed_set_.insert(new_node->id);
}

void FlowField::flow(PathNode* end)
{
    constexpr float sqrt2 = 1.41421356;
    PathNode* cur = end;
    while (true)
    {
        expand(cur->x - 1, cur->y - 1, cur, sqrt2);
        expand(cur->x - 1, cur->y, cur, 1);
        expand(cur->x - 1, cur->y + 1, cur, sqrt2);
        expand(cur->x, cur->y + 1, cur, 1);
        expand(cur->x, cur->y - 1, cur, 1);
        expand(cur->x + 1, cur->y - 1, cur, sqrt2);
        expand(cur->x + 1, cur->y, cur, 1);
        expand(cur->x + 1, cur->y + 1, cur, sqrt2);
        if (open_set_.empty())
        {
            break;
        }
        cur = open_set_.top();
        open_set_.pop();
    }
}
