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
    int x, y;
    struct Node* parent;

    Node(int x, int y)
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

    RRT(Node* s, Node* g){ start = s; goal = g; }

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

            cout << rnd.first << ' ' << rnd.second << endl;

            // find nearest node
        }
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

    // start position
    Node* start = new Node(0, 0);

    // goal position
    Node* goal = new Node(6, 7);

    RRT rrt = RRT(start, goal);

    rrt.planning(obstacles);

    return 0;
}
