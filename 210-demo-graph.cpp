// COMSC-210-5293 | Lab 34 | Yuyi Chen

#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <climits>
using namespace std;

const int SIZE = 13;
const int INF = INT_MAX;

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

    // shortestPath() finds the shortest paths using Dijkstra's algorithm
    // arguments: starting device of the network
    // returns: nothing, displays the shortest latency to each device
    void shortestPath(int start) {
        if (start < 0 || start >= SIZE || !active[start]) {
            cout << "Invalid starting device." << endl;
            return;
        }

        vector<int> distance(SIZE, INF);
        vector<bool> visited(SIZE, false);

        distance[start] = 0;

        // Find the shortest distance for each active device
        for (int count = 0; count < SIZE; count++) {
            int current = -1;
            int minDistance = INF;

            // Find the unvisited device with the shortest distance
            for (int i = 0; i < SIZE; i++) {
                if (active[i] && !visited[i] &&
                    distance[i] < minDistance) {
                    minDistance = distance[i];
                    current = i;
                }
            }

            // Stop if no more devices can be reached
            if (current == -1) {
                break;
            }

            visited[current] = true;

            // Check all neighboring devices
            for (Pair neighbor : adjList[current]) {
                int next = neighbor.first;
                int weight = neighbor.second;

                if (active[next] && !visited[next] &&
                    distance[current] <= INF - weight &&
                    distance[current] + weight < distance[next]) {
                    distance[next] = distance[current] + weight;
                }
            }
        }

        cout << "Shortest Network Latency from Device "
             << start << " (" << deviceNames[start] << "):"
             << endl;

        for (int i = 0; i < SIZE; i++) {
            if (active[i]) {
                cout << "Device " << start << " -> Device " << i
                     << " (" << deviceNames[i] << "): ";

                if (distance[i] == INF) {
                    cout << "Unreachable";
                }
                else {
                    cout << distance[i] << " ms";
                }

                cout << endl;
            }
        }
    }

    // minimumSpanningTree() finds the MST using Prim's algorithm
    // arguments: starting device of the network
    // returns: nothing, displays MST edges and total weight
    void minimumSpanningTree(int start) {
        if (start < 0 || start >= SIZE || !active[start]) {
            cout << "Invalid starting device." << endl;
            return;
        }

        vector<bool> inMST(SIZE, false);
        int totalWeight = 0;
        int edgeCount = 0;
        int activeCount = 0;

        // Count the active devices
        for (int i = 0; i < SIZE; i++) {
            if (active[i]) {
                activeCount++;
            }
        }

        // Start the MST from the selected device
        inMST[start] = true;

        cout << "Minimum Spanning Tree (Prim's Algorithm):"
             << endl;

        // Add one new device to the MST each time
        while (edgeCount < activeCount - 1) {
            int minWeight = INF;
            int from = -1;
            int to = -1;

            // Search all devices already in the MST
            for (int i = 0; i < SIZE; i++) {
                if (active[i] && inMST[i]) {
                    for (Pair neighbor : adjList[i]) {
                        int next = neighbor.first;
                        int weight = neighbor.second;

                        if (active[next] && !inMST[next] &&
                            weight < minWeight) {
                            minWeight = weight;
                            from = i;
                            to = next;
                        }
                    }
                }
            }

            // Stop if the remaining devices cannot be reached
            if (to == -1) {
                cout << "Network is disconnected." << endl;
                return;
            }

            // Add the selected connection to the MST
            inMST[to] = true;
            totalWeight += minWeight;
            edgeCount++;

            cout << "Device " << from << " ("
                 << deviceNames[from] << ") -> Device "
                 << to << " (" << deviceNames[to] << ")"
                 << " - Weight: " << minWeight << endl;
        }

        cout << "Total MST Weight: " << totalWeight << endl;
        cout << "Total Connections: " << edgeCount << endl;
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
    cout << endl;

    // Find shortest network latency from the main router
    graph.shortestPath(0);
    cout << endl;

    // Find the minimum spanning tree of the network
    graph.minimumSpanningTree(0);

    return 0;
}
