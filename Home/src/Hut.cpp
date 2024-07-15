#include <iostream>
#include <vector>
#include <cmath>
#include <climits>
#include <functional>
#include <ctime>
#include <cstdlib>
#include <graphics.h>

using namespace std;

struct Point {
    int x, y;
};

// Hàm tính khoảng cách Euclid giữa hai điểm
double distance(Point a, Point b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Hàm vẽ một điểm trên màn hình đồ họa
void drawPoint(Point p, int color) {
    setcolor(color);
    circle(p.x, p.y, 5);
    floodfill(p.x, p.y, color);
}

// Hàm vẽ một đường thẳng nối hai điểm và hiển thị trọng số
void drawLine(Point a, Point b, int color, double weight) {
    setcolor(color);
    line(a.x, a.y, b.x, b.y);
    
    // Tính tọa độ trung điểm để hiển thị trọng số
    int midX = (a.x + b.x) / 2;
    int midY = (a.y + b.y) / 2;
    
    char weightStr[10];
    sprintf(weightStr, "%.2f", weight); // Chuyển trọng số thành chuỗi
    outtextxy(midX, midY, weightStr); // Vẽ trọng số tại trung điểm
}

// Hàm tìm chu trình ngắn nhất với trọng số sử dụng phương pháp Branch and Bound
void TSP(vector<Point>& points, vector<int>& bestPath, double& minDist, vector<vector<double>>& weights) {
    int n = points.size();
    vector<int> path(n + 1);
    vector<bool> visited(n, false);
    vector<double> minDistToPoint(n, INT_MAX);

    for (int i = 0; i < n; ++i) {
        path[i] = i;
    }

    function<void(int, int, double)> search = [&](int level, int current, double currentDist) {
        if (currentDist >= minDist) return;

        if (level == n) {
            double totalDist = currentDist + weights[current][0];
            if (totalDist < minDist) {
                minDist = totalDist;
                for (int i = 0; i < n; ++i) {
                    bestPath[i] = path[i];
                }
                bestPath[n] = 0;
            }
            return;
        }

        for (int i = 1; i < n; ++i) {
            if (!visited[i] && currentDist + weights[current][i] < minDistToPoint[i]) {
                path[level] = i;
                visited[i] = true;
                minDistToPoint[i] = currentDist + weights[current][i];
                search(level + 1, i, currentDist + weights[current][i]);
                visited[i] = false;
            }
        }
    };

    visited[0] = true;
    search(1, 0, 0);
}

int main() {
    int n;
    cout << "Nhap so luong buu dien: ";
    cin >> n;

    srand(time(0));

    vector<Point> points(n);
    vector<vector<double>> weights(n, vector<double>(n));

    for (int i = 0; i < n; ++i) {
        points[i].x = rand() % 800;
        points[i].y = rand() % 600;
        cout << "Toa do cua buu dien " << i + 1 << ": (" << points[i].x << ", " << points[i].y << ")" << endl;
    }

    // Tính toán trọng số cho mỗi cạnh (giới hạn từ 1 đến 9)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                weights[i][j] = distance(points[i], points[j]) + (rand() % 9 + 1); // Trọng số từ 1 đến 9
            } else {
                weights[i][j] = 0; // Không có trọng số cho chính nó
            }
        }
    }

    vector<int> bestPath(n + 1);
    double minDist = INT_MAX;
    TSP(points, bestPath, minDist, weights);

    cout << "Phuong an tim duoc: ";
    for (int i = 0; i < n + 1; ++i) {
        cout << bestPath[i] + 1 << " ";
    }
    cout << endl;
    cout << "Tong do dai: " << minDist << endl;

    // Vẽ đồ họa
    initwindow(800, 600);
    for (int i = 0; i < n; ++i) {
        drawPoint(points[i], WHITE);
    }

    for (int i = 0; i < n; ++i) {
        drawLine(points[bestPath[i]], points[bestPath[i + 1]], YELLOW, weights[bestPath[i]][bestPath[i + 1]]);
        delay(500);  // Chờ một chút để thấy rõ
    }
    drawLine(points[bestPath[n]], points[bestPath[0]], YELLOW, weights[bestPath[n]][bestPath[0]]);  // Đóng vòng
    delay(2000);
    closegraph();

    return 0;
}
