#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib>  // for rand() and srand()
#include <ctime>    // for time()
#include <graphics.h> // Include graphics.h if using a library like WinBGIm
#include <cstdio>
#include <cmath>    // for sin() and cos()

using namespace std;

const int INF = numeric_limits<int>::max();
const int COLORS[] = {RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

struct Graph {
    int V;
    vector<vector<int>> adjMatrix;
};

void initGraph(Graph &g, int V) {
    g.V = V;
    g.adjMatrix.assign(V, vector<int>(V, INF));
}

void addEdge(Graph &g, int u, int v, int w) {
    g.adjMatrix[u][v] = w;
    g.adjMatrix[v][u] = w;
}

int tsp(const Graph &g, vector<int> &path, int pos, int visited, vector<vector<int>> &dp) {
    if (visited == (1 << g.V) - 1) {
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
    char driver[] = "";  // Sửa đổi này loại bỏ cảnh báo
    initgraph(&gd, &gm, driver);

    int width = getmaxx();
    int height = getmaxy();
    int radius = min(width, height) / 3;
    int centerX = width / 2;
    int centerY = height / 2;

    vector<pair<int, int>> positions(g.V);
    for (int i = 0; i < g.V; ++i) {
        double angle = 2 * M_PI * i / g.V;
        int x = centerX + radius * cos(angle);
        int y = centerY + radius * sin(angle);
        positions[i] = {x, y};
        circle(x, y, 20);

        char label[10];
        sprintf(label, "%d", i);
        outtextxy(x - 10, y - 10, label);
    }

    for (int u = 0; u < g.V; ++u) {
        for (int v = 0; v < g.V; ++v) {
            if (u != v && g.adjMatrix[u][v] != INF) {
                int colorIndex = rand() % (sizeof(COLORS) / sizeof(COLORS[0]));
                setcolor(COLORS[colorIndex]);  // Dòng lệnh thiết lập màu sắc cho cạnh

                int midX = (positions[u].first + positions[v].first) / 2;
                int midY = (positions[u].second + positions[v].second) / 2;
                char weightLabel[10];
                sprintf(weightLabel, "%d", g.adjMatrix[u][v]);
                line(positions[u].first, positions[u].second, positions[v].first, positions[v].second);
                outtextxy(midX, midY, weightLabel);
            }
        }
    }

    getch();
    closegraph();
}

void generateRandomMatrix(Graph &g, int maxDistance) {
    srand(time(0));  // Initialize random number generator
    for (int i = 0; i < g.V; ++i) {
        for (int j = i; j < g.V; ++j) {
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

void primMST(const Graph &g) {
    vector<int> key(g.V, INF);
    vector<bool> inMST(g.V, false);
    vector<int> parent(g.V, -1);

    key[0] = 0;

    for (int count = 0; count < g.V - 1; ++count) {
        int minKey = INF, u;

        for (int v = 0; v < g.V; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        inMST[u] = true;

        for (int v = 0; v < g.V; ++v) {
            if (g.adjMatrix[u][v] && !inMST[v] && g.adjMatrix[u][v] < key[v]) {
                key[v] = g.adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    cout << "Edges in MST:" << endl;
    for (int i = 1; i < g.V; ++i) {
        cout << parent[i] << " - " << i << " with weight " << g.adjMatrix[i][parent[i]] << endl;
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
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            cout << g.adjMatrix[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> path(V, -1);
    vector<vector<int>> dp(V, vector<int>((1 << V), -1));
    int res = tsp(g, path, 0, 1, dp);

    cout << "Minimum travel distance: " << res << endl;

    drawGraph(g, path);

    primMST(g);

    return 0;
}
