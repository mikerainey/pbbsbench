#pragma once

#include <iostream>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include "../pbbslib/parallel.h"
using namespace std;

using DefaultIntV = int;

// *************************************************************
//    POINTS AND VECTORS (3d),  2d is below
// *************************************************************

template <class coord> class point3d;

template <class coord> class vector3d {
public:
  using coordT = coord;
  using vectorT = vector3d<coord>;
  using pointT = point3d<coord>;
  coord x; coord y; coord z;
  vector3d(coord xx,coord yy, coord zz) : x(xx),y(yy),z(zz) {}
  vector3d() {x=0;y=0;z=0;}
  vector3d(pointT p);
  vector3d(coord* p) : x(p[0]), y(p[1]), z(p[2]) {};
  vectorT operator+(vectorT op2) {
    return vectorT(x + op2.x, y + op2.y, z + op2.z);}
  vectorT operator-(vectorT op2) {
    return vectorT(x - op2.x, y - op2.y, z - op2.z);}
  pointT operator+(pointT op2);
  vectorT operator*(coord s) {return vectorT(x * s, y * s, z * s);}
  vectorT operator/(coord s) {return vectorT(x / s, y / s, z / s);}
  coord& operator[] (int i) {return (i==0) ? x : (i==1) ? y : z;}
  coord dot(vectorT v) {return x * v.x + y * v.y + z * v.z;}
  vectorT cross(vectorT v) {
    return vectorT(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
  }
  coord maxDim() {return max(x,max(y,z));}
  void print() {cout << std::setprecision(10) << ":(" << x << "," << y << "," << z << "):";}
  coord Length(void) { return sqrt(x*x+y*y+z*z);}
  static const int dim = 3;
};

template <class coord> class point3d {
public:
  using coordT = coord;
  using vectorT = vector3d<coord>;
  using pointT = point3d<coord>;
  coord x; coord y; coord z;
  int dimension() {return 3;}
  point3d(coord xx,coord yy, coord zz) : x(xx),y(yy),z(zz) {}
  point3d() {x=0;y=0;z=0;}
  point3d(vectorT v) : x(v.x),y(v.y),z(v.z) {};
  point3d(coord* p) : x(p[0]), y(p[1]), z(p[2]) {};
  void print() {cout << ":(" << x << "," << y << "," << z << "):";}
  vectorT operator-(pointT op2) {
    return vectorT(x - op2.x, y - op2.y, z - op2.z);}
  pointT operator+(vectorT op2) {
    return pointT(x + op2.x, y + op2.y, z + op2.z);}
  pointT minCoords(pointT b) {
    return pointT(min(x,b.x),min(y,b.y),min(z,b.z)); }
  pointT maxCoords(pointT b) { 
    return pointT(max(x,b.x),max(y,b.y),max(z,b.z)); }
  coord& operator[] (int i) {return (i==0) ? x : (i==1) ? y : z;}
  int quadrant(pointT center) {
    int index = 0;
    if (x > center.x) index += 1;
    if (y > center.y) index += 2;
    if (z > center.z) index += 4;
    return index;
  }
  // returns a pointT offset by offset in one of 8 directions 
  // depending on dir (an integer from [0..7])
  pointT offsetPoint(int dir, coord offset) {
    coord xx = x + ((dir & 1) ? offset : -offset);
    coord yy = y + ((dir & 2) ? offset : -offset);
    coord zz = z + ((dir & 4) ? offset : -offset);
    return pointT(xx,yy,zz);
  }
  // checks if pt is outside of a box centered at this point with
  // radius hsize
  bool outOfBox(pointT pt, coord hsize) { 
    return ((x - hsize > pt.x) || (x + hsize < pt.x) ||
	    (y - hsize > pt.y) || (y + hsize < pt.y) ||
	    (z - hsize > pt.z) || (z + hsize < pt.z));
  }
  static const int dim = 3;
};

template <class coord>
inline point3d<coord> vector3d<coord>::operator+(point3d<coord> op2) {
  return point3d<coord>(x + op2.x, y + op2.y, z + op2.z);}

template <class coord>
inline vector3d<coord>::vector3d(point3d<coord> p) { x = p.x; y = p.y; z = p.z;}

// *************************************************************
//    POINTS AND VECTORS (2d)
// *************************************************************

template <class coord> class point2d;

template <class coord> class vector2d {
public:
  using coordT = coord;
  using pointT = point2d<coord>;
  using vectorT = vector2d<coord>;
  coord x; coord y;
  vector2d(coord xx,coord yy) : x(xx),y(yy) {}
  vector2d() {x=0;y=0;}
  vector2d(pointT p);
  vector2d(coord* p) : x(p[0]), y(p[1]) {};
  vectorT operator+(vectorT op2) {return vectorT(x + op2.x, y + op2.y);}
  vectorT operator-(vectorT op2) {return vectorT(x - op2.x, y - op2.y);}
  pointT operator+(pointT op2);
  vectorT operator*(coord s) {return vectorT(x * s, y * s);}
  vectorT operator/(coord s) {return vectorT(x / s, y / s);}
  coord operator[] (int i) {return (i==0) ? x : y;};
  coord dot(vectorT v) {return x * v.x + y * v.y;}
  coord cross(vectorT v) { return x*v.y - y*v.x; }  
  coord maxDim() {return max(x,y);}
  void print() {cout << ":(" << x << "," << y << "):";}
  coord Length(void) { return sqrt(x*x+y*y);}
  static const int dim = 2;
};

template <class coord>
static std::ostream& operator<<(std::ostream& os, const vector3d<coord> v) {
  return os << v.x << " " << v.y << " " << v.z; }

template <class coord>
static std::ostream& operator<<(std::ostream& os, const point3d<coord> v) {
  return os << v.x << " " << v.y << " " << v.z;
}

template <class coord> class point2d {
public:
  using coordT = coord;
  using vectorT = vector2d<coord>;
  using pointT = point2d<coord>;
  coord x; coord y; 
  int dimension() {return 2;}
  point2d(coord xx,coord yy) : x(xx),y(yy) {}
  point2d() {x=0;y=0;}
  point2d(vectorT v) : x(v.x),y(v.y) {};
  point2d(coord* p) : x(p[0]), y(p[1]) {};
  void print() {cout << ":(" << x << "," << y << "):";}
  vectorT operator-(pointT op2) {return vectorT(x - op2.x, y - op2.y);}
  pointT operator+(vectorT op2) {return pointT(x + op2.x, y + op2.y);}
  coord operator[] (int i) {return (i==0) ? x : y;};
  pointT minCoords(pointT b) { return pointT(min(x,b.x),min(y,b.y)); }
  pointT maxCoords(pointT b) { return pointT(max(x,b.x),max(y,b.y)); }
  int quadrant(pointT center) {
    int index = 0;
    if (x > center.x) index += 1;
    if (y > center.y) index += 2;
    return index;
  }
  // returns a pointT offset by offset in one of 4 directions 
  // depending on dir (an integer from [0..3])
  pointT offsetPoint(int dir, coord offset) {
    coord xx = x + ((dir & 1) ? offset : -offset);
    coord yy = y + ((dir & 2) ? offset : -offset);
    return pointT(xx,yy);
  }
  bool outOfBox(pointT pt, coord hsize) { 
    return ((x - hsize > pt.x) || (x + hsize < pt.x) ||
	    (y - hsize > pt.y) || (y + hsize < pt.y));
  }
  static const int dim = 2;
};

template <class coord>
inline point2d<coord> vector2d<coord>::operator+(point2d<coord> op2) {
  return point2d<coord>(x + op2.x, y + op2.y);}

template <class coord>
inline vector2d<coord>::vector2d(point2d<coord> p) { x = p.x; y = p.y;}

template <class coord>
static std::ostream& operator<<(std::ostream& os, const vector2d<coord> v) {
  return os << v.x << " " << v.y;}

template <class coord>
static std::ostream& operator<<(std::ostream& os, const point2d<coord> v) {
  return os << v.x << " " << v.y; }

// *************************************************************
//    GEOMETRY
// *************************************************************

// Returns twice the area of the oriented triangle (a, b, c)
template <class coord>
inline coord triArea(point2d<coord> a, point2d<coord> b, point2d<coord> c) {
  return (b-a).cross(c-a);
}

template <class coord>
inline coord triAreaNormalized(point2d<coord> a, point2d<coord> b, point2d<coord> c) {
  return triArea(a,b,c)/((b-a).Length()*(c-a).Length());
}

// Returns TRUE if the points a, b, c are in a counterclockise order
template <class coord>
inline int counterClockwise(point2d<coord> a, point2d<coord> b, point2d<coord> c) {
  return (b-a).cross(c-a) > 0.0;
}

template <class coord>
inline vector3d<coord> onParabola(vector2d<coord> v) {
  return vector3d<coord>(v.x, v.y, v.x*v.x + v.y*v.y);}

// Returns TRUE if the point d is inside the circle defined by the
// points a, b, c. 
// Projects a, b, c onto a parabola centered with d at the origin
//   and does a plane side test (tet volume > 0 test)
template <class coord>
inline bool inCircle(point2d<coord> a, point2d<coord> b, 
		      point2d<coord> c, point2d<coord> d) {
  vector3d<coord> ad = onParabola(a-d);
  vector3d<coord> bd = onParabola(b-d);
  vector3d<coord> cd = onParabola(c-d);
  return (ad.cross(bd)).dot(cd) > 0.0;
}

// returns a number between -1 and 1, such that -1 is out at infinity,
// positive numbers are on the inside, and 0 is at the boundary
template <class coord>
inline double inCircleNormalized(point2d<coord> a, point2d<coord> b, 
				 point2d<coord> c, point2d<coord> d) {
  vector3d<coord> ad = onParabola(a-d);
  vector3d<coord> bd = onParabola(b-d);
  vector3d<coord> cd = onParabola(c-d);
  return (ad.cross(bd)).dot(cd)/(ad.Length()*bd.Length()*cd.Length());
}

// *************************************************************
//    TRIANGLES
// *************************************************************
/*
struct triangle {
  int C[3];
  triangle(int p1, int p2, int p3) {
    C[0] = p1; C[1] = p2; C[2] = p3;
  }
};

template <class point, class intV = DefaultIntV>
struct triangles {
  intV numPoints;
  intV numTriangles;
  point* P;
  triangle* T;
  triangles() {}
  void del() {free(P); free(T);}
  triangles(intV np, intV nt, point* _P, triangle* _T) 
    : numPoints(np), numTriangles(nt), P(_P), T(_T) {}
};

template <class pointT>
struct ray {
  typedef typename pointT::vectorT vectorT;
  pointT o;
  vectorT d;
  ray(pointT _o, vectorT _d) : o(_o), d(_d) {}
  ray() {}
};

inline double angle(point2d a, point2d b, point2d c) {
  vector2d ba = (b-a);
  vector2d ca = (c-a);
  double lba = ba.Length();
  double lca = ca.Length();
  double pi = 3.14159;
  return 180/pi*acos(ba.dot(ca)/(lba*lca));
}

inline double minAngleCheck(point2d a, point2d b, point2d c, double angle) {
  vector2d ba = (b-a);
  vector2d ca = (c-a);
  vector2d cb = (c-b);
  double lba = ba.Length();
  double lca = ca.Length();
  double lcb = cb.Length();
  double pi = 3.14159;
  double co = cos(angle*pi/180.);
  return (ba.dot(ca)/(lba*lca) > co || ca.dot(cb)/(lca*lcb) > co || 
	  -ba.dot(cb)/(lba*lcb) > co);
}

inline point2d triangleCircumcenter(point2d a, point2d b, point2d c) {
  vector2d v1 = b-a;
  vector2d v2 = c-a;
  vector2d v11 = v1 * v2.dot(v2);
  vector2d v22 = v2 * v1.dot(v1);
  return  a + vector2d(v22.y - v11.y, v11.x - v22.x)/(2.0 * v1.cross(v2));
}

*/
