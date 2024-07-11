#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#include <graphics.h> // Include graphics.h if using a library like WinBGIm
#include <cstdio>
#include <cmath>    // for sin() and cos()

using namespace std;

const int INF = numeric_limits<int>::max();

struct Graph {
    int V;
    vector<vector<int>> adjMatrix;
};

void initGraph(Graph &g, int V) {
    g.V = V;
    g.adjMatrix = vector<vector<int>>(V, vector<int>(V, INF));
}

void addEdge(Graph &g, int u, int v, int w) {
    g.adjMatrix[u][v] = w;
    g.adjMatrix[v][u] = w;
}

int tsp(const Graph &g, vector<int> &path, int pos, int visited, vector<vector<int>> &dp) {
    if (visited == ((1 << g.V) - 1)) {
        return g.adjMatrix[pos][0]; // return to the starting point
    }
    
    if (dp[pos][visited] != -1) {
        return dp[pos][visited];
    }
    
    int res = INF;
    for (int i = 0; i < g.V; ++i) {
        if ((visited & (1 << i)) == 0 && g.adjMatrix[pos][i] != INF) {
            int newRes = g.adjMatrix[pos][i] + tsp(g, path, i, visited | (1 << i), dp);
            if (newRes < res) {
                res = newRes;
                path[pos] = i;
            }
        }
    }
    
    return dp[pos][visited] = res;
}

void drawGraph(const Graph &g, const vector<int> &bestPath) {
    int gd = DETECT, gm;
    char driver[] = "";
    initgraph(&gd, &gm, driver);

    int width = getmaxx();
    int height = getmaxy();
    int radius = min(width, height) / 3;

    int centerX = width / 2;
    int centerY = height / 2;

    vector<pair<int, int>> positions(g.V);
    for (int i = 0; i < g.V; i++) {
        double angle = 2 * M_PI * i / g.V;
        int x = centerX + radius * cos(angle);
        int y = centerY + radius * sin(angle);
        positions[i] = {x, y};
        circle(x, y, 20);

        char label[10];
        sprintf(label, "%d", i);
        outtextxy(x - 10, y - 10, label);
    }

    for (int i = 0; i < g.V; i++) {
        int u = i;
        int v = bestPath[i];
        if (v != -1 && g.adjMatrix[u][v] != INF) {
            line(positions[u].first, positions[u].second, positions[v].first, positions[v].second);
        }
    }

    getch();
    closegraph();
}

void generateRandomMatrix(Graph &g, int maxDistance) {
    srand(time(0));  // Initialize random number generator
    for (int i = 0; i < g.V; i++) {
        for (int j = i; j < g.V; j++) {
            if (i == j) {
                g.adjMatrix[i][j] = 0;
            } else {
                int randomDistance = rand() % maxDistance + 1;
                g.adjMatrix[i][j] = randomDistance;
                g.adjMatrix[j][i] = randomDistance; // Ensure symmetry
            }
        }
    }
}

int main() {
    int V;
    cout << "Enter number of post offices: ";
    cin >> V;

    Graph g;
    initGraph(g, V);

    int maxDistance;
    cout << "Enter the maximum distance between post offices: ";
    cin >> maxDistance;

    generateRandomMatrix(g, maxDistance);

    cout << "Random distance matrix:" << endl;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            cout << g.adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> path(V, -1);
    vector<vector<int>> dp(V, vector<int>((1 << V), -1));
    int res = tsp(g, path, 0, 1, dp);

    cout << "Minimum travel distance: " << res << endl;

    drawGraph(g, path);

    return 0;
}
