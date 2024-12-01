//
// Created by root on 11/30/24.
//

#include "FlowField.h"

#include <float.h>

FlowField::FlowField(const float to_x, const float to_y, const movementType movement): movement_(movement)
{
    int tile_x=MapConfig::x_in_which(to_x);
    int tile_y=MapConfig::y_in_which(to_y);
    field.resize(MapConfig::world_width);
    for (int i = 0; i < MapConfig::world_width; ++i)
    {
        field[i].resize(MapConfig::world_height);
    }
    if (get_cost(tile_x,tile_y) == -1)
    {
        return;
    }
    nodes_.resize(MapConfig::world_width);
    for (int i = 0; i < MapConfig::world_width; ++i)
    {
        nodes_[i].resize(MapConfig::world_height);
        for (int j = 0; j < MapConfig::world_height; ++j)
        {
            nodes_[i][j] = new PathNode(i, j, nullptr);
        }
    }

    const auto end = new PathNode(tile_x,tile_y, nullptr);
    flow(end);

    for (int i = 0; i < MapConfig::world_width; ++i)
    {
        for (int j = 0; j < MapConfig::world_height; ++j)
        {
            float x = 0;
            float y = 0;

            if (i > 0 && j > 0 && nodes_[i - 1][j - 1] != nullptr)
            {
                x += nodes_[i - 1][j - 1]->f - nodes_[i][j]->f;;
                y += nodes_[i - 1][j - 1]->f - nodes_[i][j]->f;;
            }
            if (i > 0 && nodes_[i - 1][j] != nullptr)
            {
                x += nodes_[i - 1][j]->f - nodes_[i][j]->f;;
            }
            if (j > 0 && nodes_[i][j - 1] != nullptr)
            {
                y += nodes_[i][j - 1]->f - nodes_[i][j]->f;;
            }
            if (i < MapConfig::world_width - 1 && j > 0 && nodes_[i + 1][j - 1] != nullptr)
            {
                x -= nodes_[i + 1][j - 1]->f - nodes_[i][j]->f;;
                y += nodes_[i + 1][j - 1]->f - nodes_[i][j]->f;;
            }
            if (i > 0 && j < MapConfig::world_height - 1 && nodes_[i - 1][j + 1] != nullptr)
            {
                x += nodes_[i - 1][j + 1]->f - nodes_[i][j]->f;;
                y -= nodes_[i - 1][j + 1]->f - nodes_[i][j]->f;;
            }
            if (i < MapConfig::world_width - 1 && nodes_[i + 1][j] != nullptr)
            {
                x -= nodes_[i + 1][j]->f - nodes_[i][j]->f;;
            }
            if (j < MapConfig::world_height - 1 && nodes_[i][j + 1] != nullptr)
            {
                y -= nodes_[i][j + 1]->f - nodes_[i][j]->f;;
            }
            if (i < MapConfig::world_width - 1 && j < MapConfig::world_height - 1 && nodes_[i + 1][j + 1] != nullptr)
            {
                x -= nodes_[i + 1][j + 1]->f - nodes_[i][j]->f;;
                y -= nodes_[i + 1][j + 1]->f - nodes_[i][j]->f;;
            }
            field[i][j].setX(x);
            field[i][j].setY(y);
            field[i][j].normalize();
        }
    }

    for (const auto n : nodes_)
    {
        for (auto path_node : n)
        {
            delete path_node;
        }
    }
    delete end;
}

float FlowField::get_cost(const int x, const int y) const
{
    if (x < 0 || y < 0 || x >= MapConfig::world_width || y >= MapConfig::world_height)
    {
        return 0;
    }
    if (closed_set_.contains(PathNode::hash(x, y)))
    {
        return 0;
    }
    if (movement_ & MapConfig::tile_attributes.at(x).at(y)->type)
    {
        return MapConfig::tile_attributes.at(x).at(y)->pass_cost;
    }
    else
    {
        return -1;
    }
}

void FlowField::expand(int x, int y, PathNode* parent, float cost_multiply)
{
    const float cost = get_cost(x, y);
    if (cost == 0)return;
    const auto new_node = nodes_[x][y];
    if (cost == -1)
    {
        new_node->f = 5211324;
    }
    else
    {
        new_node->f = parent->f + cost * cost_multiply;
    }
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
