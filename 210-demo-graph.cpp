// COMSC-210-5293 | Lab 34 | Yuyi Chen

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
using namespace std;

const int SIZE = 13;

struct Edge {
    int src, dest, weight;
};

typedef pair<int, int> Pair;  // Creates alias 'Pair' for the pair<int,int> data type

class Graph {
public:
    // a vector of vectors of Pairs to represent an adjacency list
    vector<vector<Pair>> adjList;

    // Track which vertices are active in the graph
    vector<bool> active;

    // Store the name of each network device
    vector<string> deviceNames;

    // Graph Constructor
    Graph(vector<Edge> const &edges) {
        // resize the vector to hold SIZE elements of type vector<Edge>
        adjList.resize(SIZE);

        // Initialize device names
        deviceNames.resize(SIZE);

        deviceNames[0] = "Main Router";
        deviceNames[2] = "Core Switch";
        deviceNames[4] = "Web Server";
        deviceNames[5] = "Database Server";
        deviceNames[6] = "Backup Server";
        deviceNames[7] = "Wireless Router";
        deviceNames[8] = "Office Switch";
        deviceNames[9] = "Workstation A";
        deviceNames[10] = "Workstation B";
        deviceNames[11] = "Firewall";
        deviceNames[12] = "Storage Server";

        // Initialize all vertices as inactive
        active.resize(SIZE, false);

        // Mark vertices that appear in an edge as active
        for (Edge edge : edges) {
            active[edge.src] = true;
            active[edge.dest] = true;
        }

        // add edges to the directed graph
        for (auto &edge: edges) {
            int src = edge.src;
            int dest = edge.dest;
            int weight = edge.weight;

            // insert at the end
            adjList[src].push_back(make_pair(dest, weight));
            // for an undirected graph, add an edge from dest to src also
            adjList[dest].push_back(make_pair(src, weight));
        }
    }

    // printGraph() displays the computer network connections
    // arguments: none
    // returns: nothing, displays devices and their connections
    void printGraph() {
        cout << "Computer Network Topology:" << endl;
        cout << "==========================" << endl;

        for (int i = 0; i < SIZE; i++) {
            if (active[i]) {
                cout << "Device " << i << " ("
                     << deviceNames[i] << ") connects to:" << endl;

                for (Pair v : adjList[i]) {
                    cout << "  -> Device " << v.first
                         << " (" << deviceNames[v.first] << ")"
                         << " - Latency: " << v.second
                         << " ms" << endl;
                }
            }
        }
    }

    // DFS() traverses the graph using depth-first search
    // arguments: starting vertex of the traversal
    // returns: nothing, displays the visited vertices
    void DFS(int start) {
        if (start < 0 || start >= SIZE || !active[start]) {
            cout << "Invalid starting vertex." << endl;
            return;
        }

        vector<bool> visited(SIZE, false);
        stack<int> s;

        visited[start] = true;
        s.push(start);

        cout << "Depth-First Network Inspection (DFS):" << endl;
        cout << "Starting from Device " << start << " ("
             << deviceNames[start] << ")" << endl;

        while (!s.empty()) {
            int current = s.top();
            s.pop();

            cout << "Inspecting Device " << current << " (" 
                 << deviceNames[current] << ")" << endl;

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;

                if (!visited[next]) {
                    visited[next] = true;
                    s.push(next);
                }
            }
        }

        cout << "Network inspection completed." << endl;
    }

    // BFS() traverses the graph using breadth-first search
    // arguments: starting vertex of the traversal
    // returns: nothing, displays the visited vertices
    void BFS(int start) {
        if (start < 0 || start >= SIZE || !active[start]) {
            cout << "Invalid starting vertex." << endl;
            return;
        }

        vector<bool> visited(SIZE, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Layer-by-Layer Network Scan (BFS):" << endl;
        cout << "Starting from Device " << start << " ("
             << deviceNames[start] << ")" << endl;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            cout << "Scanning Device " << current << " (" 
                 << deviceNames[current] << ")" << endl;

            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;

                if (!visited[next]) {
                    visited[next] = true;
                    q.push(next);
                }
            }
        }

        cout << "Network scan completed." << endl;
    }
};

int main() {
    // Creates a vector of graph edges/weights
    vector<Edge> edges = {
        // (x, y, w) —> edge from x to y having weight w
        // Original nodes with updated weights
        {0,2,10}, {2,6,4}, {5,6,7}, {4,5,12}, {2,4,6}, {2,5,9},

        // Connections to six new nodes
        {0,7,8}, {2,8,5}, {4,9,11}, {5,10,6}, {6,11,7}, {7,12,9},

        // Additional connections
        {7,8,3}, {8,9,4}, {9,10,8}, {10,11,5}, {11,12,6}
    };

    // Creates graph
    Graph graph(edges);

    // Prints adjacency list representation of graph
    graph.printGraph();
    cout << endl;

    // Test depth-first search starting from vertex 0
    graph.DFS(0);
    cout << endl;

    // Test breadth-first search starting from vertex 0
    graph.BFS(0);

    return 0;
}
