#ifndef R3GRAPH_H
#define R3GRAPH_H

#include <cmath>
#include <map>
#include <vector>

//#include "Triangulation.h"
//const double R3_EPSILON = 1.0e-8;

namespace R3Graph {

class R3Vector;
class R3Point;

const double R3_EPSILON = 1e-8;

class R3Vector {
public:
    double x;
    double y;
    double z;

    R3Vector():
        x(0.0),
        y(0.0),
        z(0.0)
    {}

    R3Vector(const R3Vector& v):
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    R3Vector(double xx, double yy, double zz):
        x(xx),
        y(yy),
        z(zz)
    {}

    R3Vector& operator=(const R3Vector& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    R3Vector operator+(const R3Vector& v) const {
        return R3Vector(x + v.x, y + v.y, z + v.z);
    }

    //R3Vector operator+=(const R3Point& v) const {
    //    return R3Vector(x + v.x, y + v.y, z + v.z);
    //}

    R3Vector& operator+=(const R3Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    //R3Vector& operator+=(const R3Point& v) {
    //    x += v.x;
    //    y += v.y;
    //    z += v.z;
    //    return *this;
    //}

    R3Vector operator-(const R3Vector& v) const {
        return R3Vector(x - v.x, y - v.y, z - v.z);
    }

    R3Vector& operator-=(const R3Vector& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    R3Vector operator-() const {
        return R3Vector(-x, -y, -z);
    }

    R3Vector& negate() {
        x = (-x);
        y = (-y);
        z = (-z);
        return *this;
    }

    R3Vector& invert() {
        return negate();
    }

    double length2() const {
        return (x*x + y*y + z*z);
    }

    double length() const {
        return sqrt(x*x + y*y + z*z);
    }

    R3Vector& normalize() {
        double l = length();
        if (fabs(l) > R3_EPSILON) {
            x /= l;
            y /= l;
            z /= l;
        }
        return *this;
    }

    R3Vector normalized() const {
        R3Vector res = *this;
        res.normalize();
        return res;
    }

    double scalarProduct(const R3Vector& v) const {
        return (x*v.x + y*v.y + z*v.z);
    }

    double operator*(const R3Vector& v) const {
        return scalarProduct(v);
    }

    R3Vector operator*(double c) const {
        return R3Vector(x*c, y*c, z*c);
    }

    R3Vector& operator*=(double c) {
        x *= c;
        y *= c;
        z *= c;
        return *this;
    }

    R3Vector vectorProduct(const R3Vector& v) const {
        return R3Vector(
            y*v.z - z*v.y,
            -x*v.z + z*v.x,
            x*v.y - y*v.x
        );
    }
    double dotProduct(const R3Vector& v) const {
        return scalarProduct(v);
    }
    R3Vector crossProduct(const R3Vector& v) const {
        return vectorProduct(v);
    }
    double norm() const { return length(); }

    static double cotan(
        const R3Vector& u, const R3Vector& v
    ) {
        double d = u.dotProduct(v);
        R3Vector c = u.crossProduct(v);
        double c_norm = c.norm();
        if (c_norm > R3_EPSILON) {
            return d / c_norm;
        }
        else {
            // must be infinity, return a big number
            return d / R3_EPSILON;
        }
    }

    static double signedVolume(
        const R3Vector& v0, const R3Vector& v1, const R3Vector& v2
    ) {
        return (
            v0.x * v1.y * v2.z +
            v0.y * v1.z * v2.x +
            v0.z * v1.x * v2.y - (
                v0.z * v1.y * v2.x +
                v0.y * v1.x * v2.z +
                v0.x * v1.z * v2.y
            )
        );
    }

    static double volume(
        const R3Vector& v0, const R3Vector& v1, const R3Vector& v2
    ) {
        return fabs(signedVolume(v0, v1, v2));
    }

    double angle(const R3Vector& v) const {
        double len0 = length();
        double len1 = v.length();

        /*...
        if (len0 > R3_EPSILON && len1 > R3_EPSILON)
            return acos(
                scalarProduct(v) / (len0*len1)
            );
        else
            return 0.;
        ...*/

        if (len0 <= R3_EPSILON || len1 <= R3_EPSILON)
            return 0;
        R3Vector ex = (*this) * (1./len0);
        R3Vector w = v * (1./len1);
        R3Vector ez = ex.vectorProduct(w);
        double lenz = ez.length();
        if (lenz <= R3_EPSILON)
            return 0.;
        ez *= (1./lenz);
        R3Vector ey = ez.vectorProduct(ex);
        return fabs(atan2(
            w*ey, w*ex
        ));
    }

    static double angle(
        const R3Vector& v0, const R3Vector& v1
    ) {
        return v0.angle(v1);
    }

    double area(const R3Vector& v) const {
        R3Vector w = vectorProduct(v);
        return w.length();
    }

    static double area(
        const R3Vector& v0, const R3Vector& v1
    ) {
        return v0.area(v1);
    }
    double signedSolidAngle(
        const R3Vector& a,
        const R3Vector& b,
        const R3Vector& c);
};

inline R3Vector operator*(double c, const R3Vector& v) {
    return R3Vector(c*v.x, c*v.y, c*v.z);
}

class R3Point {
public:
    double x;
    double y;
    double z;

    R3Point():
        x(0.0),
        y(0.0),
        z(0.0)
    {}

    R3Point(const R3Point& v):
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    R3Point(const R3Vector& v) :
        x(v.x),
        y(v.y),
        z(v.z)
    {}

    R3Point(double xx, double yy, double zz):
        x(xx),
        y(yy),
        z(zz)
    {}

    R3Point& operator=(const R3Point& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    bool operator==(const R3Point& v) const {
        return (
            fabs(x - v.x) <= R3_EPSILON &&
            fabs(y - v.y) <= R3_EPSILON &&
            fabs(z - v.z) <= R3_EPSILON
        );
    }

    bool operator!=(const R3Point& v) const {
        return !operator==(v);
    }

    R3Point operator+(const R3Vector& v) const {
        return R3Point(x + v.x, y + v.y, z + v.z);
    }

    R3Point operator+(const R3Point& p) const {
        return R3Point(x + p.x, y + p.y, z + p.z);
    }

    //... R3Point operator+(const R3Point& v) const {
    //...     return R3Point(x + v.x, y + v.y, z + v.z);
    //... }

    R3Point& operator+=(const R3Vector& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    R3Point& operator+=(const R3Point& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    R3Vector operator-(const R3Point& v) const {
        return R3Vector(x - v.x, y - v.y, z - v.z);
    }

    R3Point operator-(const R3Vector& v) const {
        return R3Point(x - v.x, y - v.y, z - v.z);
    }

    R3Point& operator-=(const R3Vector& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    //... R3Point& operator-=(const R3Point& v) {
    //...     x -= v.x;
    //...     y -= v.y;
    //...     z -= v.z;
    //...     return *this;
    //... }

    //... R3Point operator-() const {
    //...     return R3Point(-x, -y, -z);
    //... }

    //... R3Point& negate() {
    //...     x = (-x);
    //...     y = (-y);
    //...     z = (-z);
    //...     return *this;
    //... }

    R3Point operator*(double c) const {
        return R3Point(x*c, y*c, z*c);
    }

    R3Point operator*=(double c) const {
        return R3Point(x * c, y * c, z * c);
    }
    //... R3Point& operator*=(double c) {
    //...     x *= c;
    //...     y *= c;
    //...     z *= c;
    //...     return *this;
    //... }

    double length()
    {
        return sqrt(x * x + y * y + z * z);
    }

    double distance(const R3Point& p) const {
        return (p - *this).length();
    }

    static double distance(
        const R3Point& p0, const R3Point& p1
    ) {
        return p0.distance(p1);
    }
};

class R3Box {
public:
    R3Point origin;
    R3Vector size;

    R3Box():
        origin(),
        size()
    {}

    R3Box(const R3Point& o, const R3Vector& s):
        origin(o),
        size(s)
    {}

    bool contains(const R3Point& p) const {
        return(
            origin.x <= p.x && p.x <= origin.x + size.x &&
            origin.y <= p.y && p.y <= origin.y + size.y &&
            origin.z <= p.z && p.z <= origin.z + size.z
        );
    }
}; 

bool intersectPlanes(
    const R3Point& p0, const R3Vector& n0,
    const R3Point& p1, const R3Vector& n1,
    R3Point& p, R3Vector& v
);

bool intersectPlaneAndLine(
    const R3Point& p0, const R3Vector& n,
    const R3Point& p1, const R3Vector& v,
    R3Point& p
);

class Edge
{
public:
    //R3Point A, B;
    // two points with threshold function meaning
    std::pair<R3Point, double> A, B;
    int index = 0;

    Edge() : 
        A(), B() {}
    
    //Edge(R3Point a, R3Point b) :
    //    A(a), B(b) {}
    Edge(std::pair<R3Point, double> a, std::pair<R3Point, double> b) :
        A(a), B(b) {}

    Edge(R3Point p1, double f_p1, R3Point p2, double f_p2)
    {
        A.first = p1;
        A.second = f_p1;
        B.first = p2;
        B.second = f_p2;
    }

    Edge& operator=(const Edge& edge) {
        A = edge.A;
        B = edge.B;
        return *this;
    }

    //bool operator<(const Edge& e) {
    //    return index < e.index;
    //}

    void SwapVerticies() {
        std::pair<R3Point, double> temp = A;
        A = B;
        B = temp;
        return;
    }

    R3Point PointOnEdge() 
    {
        // thrfunA
        // |\
        // | \              A, B - points in 3D
        // |  \
        // |   \ iso      B
        // |----\---------|
        // A     \ point  |
        //        \       |
        //         \      |
        //          \     |thrfunB
        //           \    |
        //            \   |
        //             \  |
        //              \ |
        //               \| 
        //return A.first + (B.first - A.first) * 0.5;
         
        if (A.second < 0.)
            SwapVerticies();

        R3Point pointA = A.first;
        double thresholdFunctionA = fabs(A.second);

        R3Point pointB = B.first;
        double thresholdFunctionB = fabs(B.second);

        R3Point signChange;
        if (thresholdFunctionA < R3_EPSILON)
        {
            signChange = pointA;
        }
        else if (thresholdFunctionB < R3_EPSILON)
        {
            signChange = pointB;
        }
        else
        {
            R3Point argVector = pointB - pointA; // vector from end of pointA to end of pointB
            //double thresholdFunctionVariation = abs(thresholdFunctionA) + abs(thresholdFunctionB); // f(ROI) - (- |f(!ROI)|)
            signChange = pointA + argVector * 0.5;// (thresholdFunctionA / thresholdFunctionVariation); // (thrfun1 / funvar) is similar coefficient
        }
        
        return signChange;
    }
};

class DensityTetrahedron 
{
public:
    Edge edges[6];
    R3Vector Outward;

    DensityTetrahedron() {
        for (int i = 0; i < 6; ++i)
            edges[i] = Edge();
    }

    DensityTetrahedron(const R3Point& CubeCenter, double ThrFunCubeCenter,
        const R3Point& NeghbourCenter, double ThrFunNeghbourCenter,
        const R3Point& CubeVertex1, double ThrFunCubeVertex1,
        const R3Point& CubeVertex2, double ThrFunCubeVertex2)
    {
        ThrFunCubeCenter = ThrFunCubeCenter < R3_EPSILON ? 10e-9 : ThrFunCubeCenter;
        ThrFunNeghbourCenter = ThrFunNeghbourCenter < R3_EPSILON ? 10e-9 : ThrFunNeghbourCenter;
        ThrFunCubeVertex1 = ThrFunCubeVertex1 < R3_EPSILON ? 10e-9 : ThrFunCubeVertex1;
        ThrFunCubeVertex2 = ThrFunCubeVertex2 < R3_EPSILON ? 10e-9 : ThrFunCubeVertex2;

        //first vertex always in ROI cube, only second vertex may appear outside ROI
        // edges beetwen cube verticies and neighbour center
        edges[0] = Edge(CubeCenter, ThrFunCubeCenter,
            NeghbourCenter, ThrFunNeghbourCenter);
        Outward = NeghbourCenter - CubeCenter;

        edges[1] = Edge(CubeVertex1, ThrFunCubeVertex1,
            NeghbourCenter, ThrFunNeghbourCenter);

        edges[2] = Edge(CubeVertex2, ThrFunCubeVertex2,
            NeghbourCenter, ThrFunNeghbourCenter);

        // edges beetwen cube verticies themselves
        edges[3] = Edge(CubeVertex1, ThrFunCubeVertex1,
            CubeCenter, ThrFunCubeCenter);

        edges[4] = Edge(CubeVertex2, ThrFunCubeVertex2,
            CubeCenter, ThrFunCubeCenter);

        edges[5] = Edge(CubeVertex1, ThrFunCubeVertex1,
            CubeVertex2, ThrFunCubeVertex2);
    }

    DensityTetrahedron(std::pair<R3Point,double>& CubeCenterPair,
        std::pair<R3Point, double>& NeghbourCenterPair,
        std::pair<R3Point, double>& CubeVertex1Pair,
        std::pair<R3Point, double>& CubeVertex2Pair)
    {

        CubeCenterPair.second = (fabs(CubeCenterPair.second) < R3_EPSILON) ? 10e-9 : CubeCenterPair.second;
        NeghbourCenterPair.second = (fabs(NeghbourCenterPair.second) < R3_EPSILON) ? 10e-9 : NeghbourCenterPair.second;
        CubeVertex1Pair.second = (fabs(CubeVertex1Pair.second) < R3_EPSILON) ? 10e-9 : CubeVertex1Pair.second;
        CubeVertex2Pair.second = (fabs(CubeVertex2Pair.second) < R3_EPSILON) ? 10e-9 : CubeVertex2Pair.second;
        //first vertex always in ROI cube, only second vertex may appear outside ROI
        // edges beetwen cube verticies and neighbour center
        edges[0] = Edge(CubeCenterPair, NeghbourCenterPair);
        Outward = NeghbourCenterPair.first - CubeCenterPair.first;

        edges[1] = Edge(CubeVertex1Pair, NeghbourCenterPair);
        edges[2] = Edge(CubeVertex2Pair, NeghbourCenterPair);

        // edges beetwen cube verticies themselves
        edges[3] = Edge(CubeVertex1Pair, CubeCenterPair);
        edges[4] = Edge(CubeVertex2Pair, CubeCenterPair);

        edges[5] = Edge(CubeVertex1Pair, CubeVertex2Pair);
    }

};

} // end of namespace R3Graph

#endif
