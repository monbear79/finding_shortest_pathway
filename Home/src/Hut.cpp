#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <graphics.h>
#include <algorithm>
#include <numeric>
#include <random>
#include <sstream>

using namespace std;

struct Point {
    int x, y;
};

double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void drawPoint(Point p, int index, int color) {
    setcolor(color);
    circle(p.x, p.y, 5);
    floodfill(p.x, p.y, color);
    string indexStr = to_string(index + 1);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(p.x, p.y - 10, const_cast<char*>(indexStr.c_str()));
    circle(p.x, p.y, 15); // Vẽ hình tròn xung quanh các bưu điện
}

void drawLine(Point a, Point b, int color, double weight) {
    setcolor(color);
    line(a.x, a.y, b.x, b.y);
    int midX = (a.x + b.x) / 2;
    int midY = (a.y + b.y) / 2;
    char weightStr[10];
    sprintf(weightStr, "%.0f", weight);
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outtextxy(midX, midY, weightStr);
}

void drawPostman(Point p) {
    setcolor(BLUE);
    circle(p.x, p.y, 10);
    floodfill(p.x, p.y, BLUE);
}

void movePostman(Point from, Point to, const vector<Point>& points, const vector<int>& bestPath, int n, const vector<vector<double>>& weights, stringstream& pathStream, int startIndex) {
    int steps = 50;
    for (int i = 0; i <= steps; ++i) {
        int x = from.x + (to.x - from.x) * i / steps;
        int y = from.y + (to.y - from.y) * i / steps;
        cleardevice();
        for (int j = 0; j < n; ++j) {
            drawPoint(points[j], j, WHITE);
        }
        for (int j = 0; j < n; ++j) {
            drawLine(points[bestPath[j]], points[bestPath[(j + 1) % n]], YELLOW, weights[bestPath[j]][bestPath[(j + 1) % n]]);
        }
        drawPostman({x, y});
        setcolor(WHITE);
        settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
        outtextxy(400, 10, const_cast<char*>(pathStream.str().c_str()));
        delay(50);
    }
    pathStream << " --> " << to_string(bestPath[(startIndex + 1) % n] + 1);
}

void TSP(vector<Point>& points, vector<int>& bestPath, double& minDist, vector<vector<double>>& weights, int startPoint) {
    int n = points.size();
    vector<int> path(n);
    iota(path.begin(), path.end(), 0);
    swap(path[0], path[startPoint]);
    vector<int> currentPath(n);
    vector<bool> visited(n, false);
    function<void(int, int, double)> search = [&](int level, int current, double currentDist) {
        if (currentDist >= minDist) return;
        if (level == n) {
            currentDist += weights[current][path[0]];
            if (currentDist < minDist) {
                minDist = currentDist;
                bestPath = currentPath;
                bestPath.push_back(path[0]);
            }
            return;
        }
        for (int i = 1; i < n; ++i) {
            if (!visited[i]) {
                visited[i] = true;
                currentPath[level] = path[i];
                search(level + 1, path[i], currentDist + weights[current][path[i]]);
                visited[i] = false;
            }
        }
    };
    currentPath[0] = path[0];
    visited[0] = true;
    search(1, path[0], 0);
}

int main() {
    int n;
    cout << "Nhap so luong buu dien: ";
    cin >> n;

    int weightLimit;
    cout << "Nhap gioi han trong so (1 den 100): ";
    cin >> weightLimit;

    srand(time(0));

    vector<Point> points(n);
    vector<vector<double>> weights(n, vector<double>(n));

    // Giới hạn tọa độ x từ 50 đến 950 và y từ 50 đến 650
    for (int i = 0; i < n; ++i) {
        points[i].x = 50 + rand() % 950;
        points[i].y = 50 + rand() % 650;
        cout << "Toa do cua buu dien " << i + 1 << ": (" << points[i].x << ", " << points[i].y << ")" << endl;
    }

    vector<int> indices(n);
    iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    vector<Point> shuffledPoints(n);
    for (int i = 0; i < n; ++i) {
        shuffledPoints[i] = points[indices[i]];
    }
    points = shuffledPoints;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                weights[i][j] = rand() % weightLimit + 1;
            } else {
                weights[i][j] = 0;
            }
        }
    }

    vector<int> bestPath;
    double minDist = INT_MAX;

    int startPoint = rand() % n;
    TSP(points, bestPath, minDist, weights, startPoint);

    cout << "Phuong an tim duoc: ";
    for (int i = 0; i < bestPath.size(); ++i) {
        cout << bestPath[i] + 1 << " ";
    }
    cout << endl;
    cout << "Tong do dai: " << minDist << endl;

    initwindow(1000, 700); // Tăng kích thước cửa sổ đồ họa
    for (int i = 0; i < n; ++i) {
        drawPoint(points[i], i, WHITE);
    }

    stringstream pathStream;
    pathStream << to_string(bestPath[0] + 1);
    for (int i = 0; i < n; ++i) {
        movePostman(points[bestPath[i]], points[bestPath[(i + 1) % n]], points, bestPath, n, weights, pathStream, i);
        drawLine(points[bestPath[i]], points[bestPath[(i + 1) % n]], YELLOW, weights[bestPath[i]][bestPath[(i + 1) % n]]);
        delay(500);
    }

    cout << "Duong di cua nguoi dan ong: " << pathStream.str() << endl;

    delay(2000);
    closegraph();

    return 0;
}