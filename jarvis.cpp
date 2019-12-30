// Дано множество точек на плоскости(x, y).Постройте выпуклую оболочку этого множества и вычислите ее периметр.
// С помощью алгоритма Джарвиса.

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct point {
    double x;
    double y;
};

const double prec = 1e-10;

double find_perimeter(vector<point>& points) {
    double perim = 0;
    for (int i = 0; i < points.size() - 1; i++)
        perim = perim + sqrt(pow((points[i + 1].x - points[i].x), 2) +
                             pow((points[i + 1].y - points[i].y), 2));


    perim = perim + sqrt(pow((points[0].x - points[points.size() -1].x), 2) +
                         pow((points[0].y - points[points.size() - 1].y), 2));
    return perim;
};

bool is_inside(const point& a, const point &b, const point &c) {
    return (a.x <= b.x && b.x <= c.x && a.y <= b.y && b.y <= c.y) or
           (a.x >= b.x && b.x >= c.x && a.y >= b.y && b.y >= c.y);
}

double rotation(const point& a, const point &b, const point &c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

vector<point> find_min_convex_hull(const vector<point>& coordinates) {
    vector<point> result;

    int start = 0;

    for (int i = 1; i < coordinates.size(); i++)
        if (coordinates[start].y > coordinates[i].y)
            start = i;
        else if (coordinates[start].y == coordinates[i].y and coordinates[start].x > coordinates[i].x)
            start = i;

    int first = start;
    int current = start;
    do {
        int next = (current + 1) % coordinates.size();

        for (int i = 0; i < coordinates.size(); i++) {
            double val = rotation(coordinates[current], coordinates[next], coordinates[i]);

            if (next == current or (val < 0 and abs(val) >= prec) or (abs(val) < prec and
            is_inside(coordinates[current], coordinates[next], coordinates[i])))
                next = i;
        }

        current = next;

        result.push_back(coordinates[next]);

    } while (current != first);

    return result;
}

int main() {
    int n;
    point p;
    double perimeter;
    vector <point> coords;
    vector<point> res;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> p.x >> p.y;
        coords.push_back(p);
    }
    res = find_min_convex_hull(coords);
    perimeter = find_perimeter(res);
    cout.precision(9);
    cout << perimeter << endl;
    return 0;
}