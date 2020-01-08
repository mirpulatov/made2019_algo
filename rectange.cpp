/*
 * На вход подаются точки и прямоугольники. Точка задается двумя координатами (x, y).
 * Прямоугольник - четверкой чисел [left, bottom, right, top]. Точка (left, bottom) -
 * принадлежит прямоугольнику, точка (right, top) - нет. (left < right, bottom < top)
 * Для каждого прямоугольника нужно вывести, сколько добавленных точек он содержит.
 * X  в диапазоне [-180, 180)
 * Y -  [-90, 90)
 * Количество точек <= 100000, Количество прямоугольников <= 1000
 * Для решения задачи необходимо реализовать алгоритм “Geohashing”.
*/

#include <iostream>
#include <vector>

#define ll long long
using namespace std;

struct Point {
    double lon;
    double lat;
};

struct Bbox {
    double left;
    double bottom;
    double right;
    double top;

    bool Contains(const Point &point) const {
        return left <= point.lon && bottom <= point.lat &&
               point.lon < right && point.lat < top;
    }
};

class GeoHashTable {
public:
    GeoHashTable(const vector<Point> &points);

    int count_points_in_box(Bbox bbox);

private:
    vector<Point> points;
};

GeoHashTable::GeoHashTable(const vector<Point> &points) {
    this->points = points;
}

int GeoHashTable::count_points_in_box(Bbox bbox) {
    int res = 0;
    for (Point p : points) {
        if (bbox.Contains(p))
            res++;
    }
    return res;
}

int main() {
    int n;
    cin >> n;
    vector<Point> points;
    double x1, y1;
    for (int i=0;i<n;i++) {
        cin>> x1>>y1;
        Point p{x1, y1};
        points.push_back(p);
    }
    GeoHashTable geo(points);

    int m;
    cin >> m;
    double x2, y2;
    for (int i = 0;i < m; i++) {
        cin >> x1 >> y1 >> x2 >> y2;
        cout << geo.count_points_in_box({x1, y1, x2, y2}) << endl;
    }
    return 0;
}