#include <iostream>
#include <vector>
#include <random>
#include <string>

struct Point {
    const int x, y;
    Point(int x_val, int y_val) : x{ x_val }, y{ y_val } {}
    std::string point_str() const {
        std::string s = "(";
        s += std::to_string(x);
        s += ",";
        s += std::to_string(y);
        s += ")";
        return s;
    }
};

struct Line {
    const Point p1, p2;
    Line(const Point a, Point b) : p1{ a }, p2{ b } {}
    
};


class Get_Random {
    inline static std::random_device rd;
    inline static std::mt19937 generator{ rd() };
public:
    static int integer(int a, int b) {
        std::uniform_int_distribution<int> distribute{ a, b };
        return distribute(generator);
    }
    static int quadrant() {
        return integer(1, 4);
    }
    static Point point() {
        int x = integer(0, 10);
        int y = integer(0, 10);
        switch (quadrant()) {
        case 1:
            return Point{ x, y };
        case 2:
            return Point{ -x, y };
        case 3:
            return Point{ -x, -y };
        case 4:
            return Point{ x, -y };
        default:
            return Point{ 0, 0 };
        }
    }
    static Line line() {
        return Line{ point(), point() };
    }
};




int main()
{
    int n;
    std::cout << "No of lines? ";
    std::cin >> n;
    std::vector<Line> lines;
    for (int i = 0; i < n; ++i) {
        lines.push_back(Get_Random::line());
    }
    for (int i = 0; i < n; ++i) {
        std::cout << "line " << i + 1 << ": p1 = " << lines[i].p1.point_str() << " , p2 = " << lines[i].p2.point_str() << '\n';
    }
    return 0;
}