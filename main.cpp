#include <bits/stdc++.h>

using namespace std;

#define EXPAND_DISTANCE 1.0
#define GOAL_SAMPLE_RATE 5
#define MAX_INTER 500
#define MIN_RAND -2
#define MAX_RAND 15

// node (x, y, parent)
struct Node
{
    float x, y;
    int parent;

    Node(float x, float y)
    {
        this->x = x;
        this->y = y;
        this->parent = NULL;
    }
};

typedef pair<float, float> PosPair;

class RRT
{
private:

public:
    Node* start;
    Node* goal;
    vector<Node*> node_list;
    vector<PosPair> path;
    int obst_num;

    RRT(Node* s, Node* g, int n){ start = s; goal = g; obst_num = n;}

    static int nearest_node_list(vector<Node*> node_list, PosPair rnd)
    {
        int min_idx = -1;

        double min_dist, dist;

        for (int i = 0; i < node_list.size(); ++i) {
            dist = (node_list[i]->x - rnd.first) * (node_list[i]->x - rnd.first) +
                   (node_list[i]->y - rnd.second) * (node_list[i]->y - rnd.second);
            if (i == 0) {min_dist = dist; min_idx = i;}
            else
            {
                if (dist < min_dist)
                {
                    min_dist = dist;
                    min_idx = i;
                }
            }
        }

        return min_idx;
    }

    static bool check_collision(Node* node, int obstacles[][3], int obst_num)
    {
        int ox, oy, os;
        float dx, dy, d;

        for (int i = 0; i < obst_num; ++i) {
            dx = ox - node->x;
            dy = oy - node->y;
            d = sqrt(dx * dx + dy * dy);

            if (d <= os) {return false;} // collision
        }

        return true; // safe
    }

    void planning(int obstacles[][3])
    {
        // add start node
        node_list.push_back(start);

        // random function
        mt19937 mt{random_device{}()};
        uniform_int_distribution<int> dist_int(0, 100);
        uniform_real_distribution<float> dist_float(MIN_RAND, MAX_RAND);

        // sampled node
        PosPair rnd;

        // nearest node's data
        int nrst_idx;
        Node* nrst_node;

        // direction angle to new node
        float angle;

        // check goal
        float dx, dy, d;
        int last_idx;
        Node* last_node;

        while (true)
        {
            // random sampling
            if (dist_int(mt) > GOAL_SAMPLE_RATE)
            {
                rnd = make_pair(dist_float(mt), dist_float(mt));
            }
            else
            {
                rnd = make_pair(goal->x, goal->y);
            }

            // find nearest node
            nrst_idx = nearest_node_list(node_list, rnd);

            // expand tree
            nrst_node = node_list[nrst_idx];
            // direction
            angle = atan2(rnd.second - nrst_node->y, rnd.first - nrst_node->x);

            // caluculate new node
            Node* new_node = nrst_node;
            new_node->x += EXPAND_DISTANCE * cos(angle);
            new_node->y += EXPAND_DISTANCE * sin(angle);
            new_node->parent = nrst_idx;

            if (!check_collision(new_node, obstacles, obst_num)) {continue;}

            node_list.push_back(new_node);

            // check goal
            dx = new_node->x - goal->x;
            dy = new_node->y - goal->y;
            d = sqrt(dx * dx + dy * dy);
            if (d <= EXPAND_DISTANCE)
            {
                cout << "Goal!!" << endl;
                break;
            }
        }

        // print path
    }
};

int main() {
    // obstacles (x, y, size)
    int obstacles[7][3] = {
            {5, 5, 2},
            {3, 6, 2},
            {3, 8, 2},
            {3, 10, 2},
            {7, 5, 2},
            {7, 9, 2},
            {9, 5, 2}
    };

    int size = sizeof(obstacles) / sizeof(*obstacles);

    // start position
    Node* start = new Node(0, 0);

    // goal position
    Node* goal = new Node(6, 7);

    RRT rrt = RRT(start, goal, size);

    rrt.planning(obstacles);

    return 0;
}
