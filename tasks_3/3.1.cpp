#include <iostream>
#include <vector>
#include <cmath>


const double PI = 3.14159265358979323846;

class Shape {
public:
    virtual double area() const = 0;
    virtual void print() const = 0;
};

class Shape2D : public Shape {
public:
    virtual double perimeter() const = 0;
};

class Ellips : public Shape2D {
    double a;
    double b;

public:
    Ellips(double a, double b) : a(a), b(b) {}

    double area() const override {
        return std::numbers::pi * a * b;
    }

    double perimeter() const override {
        return 2 * std::numbers::pi * std::sqrt((a * a + b * b)/2);
    }

    void print() const override {
        std::cout << "Ellips: (" << a << ' ' << b << ") s = " << area()
                  << ", p = " << perimeter() << std::endl;
    }
};

class Circle : public Ellips {
    double r;
public:
    Circle(double a) : Ellips(a, a) {
        r = a;
    }

    void print() const override {
        std::cout << "Circle: (" << r << ") s = " << area()
                  << ", p = " << perimeter() << std::endl;
    }
};

class Triangle : public Shape2D {
    double side1, side2, side3;

public:
    Triangle(double s1, double s2, double s3){
        side1 = s1;
        side2 = s2;
        side3 = s3;
    }

    double area() const override{
        double s = perimeter() / 2;
        return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    double perimeter() const override {
        return side1 + side2 + side3;
    }

    void print() const override {
        std::cout << "Triangle: (" << side1 << ' ' << side2 << ' ' << side3 << ") s = " << area()
                << ", p = " << perimeter() << std::endl;
    }
};

class RectangularTriangle : public Triangle {
    double s1;
    double s2;
public:
    RectangularTriangle(double s1, double s2) : Triangle(s1, s2, std::sqrt(s1*s1 + s2*s2)) {
        this->s1 = s1;
        this->s2 = s2;
    }

    double area() const override {
        return s1 * s2 / 2;
    }

    void print() const override {
        std::cout << "Rectangular Triangle: (" << s1 << ' ' << s2 << ") s = " << area()
                  << ", p = " << perimeter() << std::endl;
    }
};

class Quadrilateral : public Shape2D {
protected:
    double a, b, c, d;

public:
    Quadrilateral(double a, double b, double c, double d) : a(a), b(b), c(c), d(d){}

    double perimeter() const override {
        return a + b + c + d;
    }

    void print() const override {
        std::cout << "Quadrilateral: (" << a << ' ' << b << ' ' << c << ' ' << d <<
            ") s = " << area() << ", p = " << perimeter() << std::endl;
    }
};

class Parallelogram : public Quadrilateral {
protected:
    double alpha;

public:
    Parallelogram(double a, double b, double alpha) : Quadrilateral(a, b, a, b), alpha(alpha){}

    double area() const override {
        return a * b * std::sin(alpha);
    }

    void print() const override {
        std::cout << "Parallelogram: (" << a << ' ' << b << ' ' << a << ' ' << b
            << ") s = " << area() << ", p = " << perimeter() << std::endl;
    }
};

class TrapezoidIsosceles : public Quadrilateral {
    double alpha;

public:
    TrapezoidIsosceles(double a, double b, double c, double alpha) :
        Quadrilateral(a, c, b, c), alpha(alpha) {}

    double area() const override {
        return (a + b) * c * std::sin(alpha) / 2;
    }

    void print() const override {
        std::cout << "TrapezoidIsosceles: (" << a << ' ' << c << ' ' << b << ' ' << c <<
                     ") alpha = " << alpha << " s = " << area() << ", p = " << perimeter() << std::endl;
    }
};

class Rectangle : public Parallelogram {

public:
    Rectangle(double a, double b) : Parallelogram(a, b, PI / 2) {}

    double area() const override {
        return a * b;
    }

    void print() const override {
        std::cout << "Rectangle: (" << a << ' ' << b << ' ' << a << ' ' << b
            << ") s = " << area() << ", p = " << perimeter() << std::endl;
    }
};

class Rhombus : public Parallelogram {

public:
    Rhombus(double a, double alpha) : Parallelogram(a, a, alpha) {}

    void print() const override {
        std::cout << "Rhombus: (" << a << ' ' << "alpha = " << alpha << ' '
            << ") s = " << area() << ", p = " << perimeter() << std::endl;
    }
};


class Shape3D : public Shape {
    virtual double volume() const = 0;
};

class Tetrahedron : public Shape3D {
    double sideAB, sideAC, sideAD, sideBC, sideCD, sideBD;
public:
    Tetrahedron(double ab, double ac, double ad, double bc, double cd, double bd) {
        sideAB = ab;
        sideAC = ac;
        sideAD = ad;
        sideBC = bc;
        sideCD = cd;
        sideBD = bd;
    }

    double area() const override {
        double sABC = (sideAB + sideAC + sideBC) / 2;
        double sABD = (sideAB + sideAD + sideBD) / 2;
        double sACD = (sideAC + sideAD + sideCD) / 2;
        double sBCD = (sideBC + sideBD + sideCD) / 2;

        return std::sqrt(sABC * (sABC - sideAB) * (sABC - sideAC) * (sABC - sideBC)) +
               std::sqrt(sABD * (sABD - sideAB) * (sABD - sideAD) * (sABD - sideBD)) +
               std::sqrt(sACD * (sACD - sideAC) * (sACD - sideAD) * (sACD - sideCD)) +
               std::sqrt(sBCD * (sBCD - sideBC) * (sBCD - sideBD) * (sBCD - sideCD));
    }

    double volume() const override {
        return std::sqrt(4 * pow(sideAB * sideAC * sideAD, 2) -
                        pow(sideAB * (sideAC * sideAC + sideAD * sideAD - sideBC * sideBC), 2) -
                        pow(sideAC * (sideAB * sideAB + sideAD * sideAD - sideCD * sideCD), 2) -
                        pow(sideAD * (sideAB * sideAB + sideAC * sideAC - sideBD * sideBD), 2) +
                        (sideAC * sideAC + sideAD * sideAD - sideBC * sideBC) *
                        (sideAB * sideAB + sideAD * sideAD - sideCD * sideCD) *
                        (sideAB * sideAB + sideAC * sideAC - sideBD * sideBD)) / 12;
    }

    void print() const override {
        std::cout << "Tetrahedron(" << sideAB << ' ' << sideAC << ' ' << sideAD << ' '
                  << sideBC << ' ' << sideCD << ' ' << sideBD << "): S = " << area()
                  << ", V = " << volume() << '\n';
    }
};

class RectangularTetrahedron : public Tetrahedron {
    double sideAB, sideAC, sideAD;
public:
    RectangularTetrahedron(double sideAB, double sideAC, double sideAD)
        : Tetrahedron(sideAB, sideAC, sideAD,
                     std::sqrt(sideAB * sideAB + sideAC * sideAC),
                     std::sqrt(sideAC * sideAC + sideAD * sideAD),
                     std::sqrt(sideAB * sideAB + sideAD * sideAD)) {
        this->sideAB = sideAB;
        this->sideAC = sideAC;
        this->sideAD = sideAD;
    }

    double area() const override {
        double sideBC = std::sqrt(sideAB * sideAB + sideAC * sideAC);
        double sideCD = std::sqrt(sideAC * sideAC + sideAD * sideAD);
        double sideBD = std::sqrt(sideAB * sideAB + sideAD * sideAD);
        double sBCD = (sideBC + sideBD + sideCD) / 2;

        return sideAB * sideAC / 2 +
               sideAB * sideAD / 2 +
               sideAC * sideAD / 2 +
               std::sqrt(sBCD * (sBCD - sideBC) * (sBCD - sideBD) * (sBCD - sideCD));
    }

    double volume() const override {
        return (sideAB * sideAC * sideAD) / 6;
    }

    void print() const override {
        std::cout << "RectangularTetrahedron(" << sideAB << ' ' << sideAC << ' ' << sideAD
                  << "): S = " << area() << ", V = " << volume() << '\n';
    }
};

class Sphere : public Shape3D {
    double R;
public:
    Sphere(double r) {
        R = r;
    }

    double area() const override {
        return 4 * std::numbers::pi * R * R;
    }

    double volume() const override {
        return 4 * std::numbers::pi * R * R * R / 3;
    }

    void print() const override {
        std::cout << "Sphere (" << R << ")" << ": S = " << area()
                  << ", V = " << volume() << '\n';
    }
};

class RectangularParallelepiped : public Shape3D {
    double a, b, c;
public:
    RectangularParallelepiped(double x, double y, double z) {
        a = x;
        b = y;
        c = z;
    }

    double area() const override {
        return 2 * (a * b + b * c + a * c);
    }

    double volume() const override {
        return a * b * c;
    }

    void print() const override {
        std::cout << "RectangularParallelepiped (" << a << ' ' << b << ' ' << c
                  << "): S = " << area() << ", V = " << volume() << '\n';
    }
};