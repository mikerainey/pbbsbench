// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2011 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "IO.h"
#include "../pbbslib/parallel.h"
#include "geometry.h"
using namespace benchIO;

template <class coord>
inline int xToStringLen(point2d<coord> a) { 
  return xToStringLen(a.x) + xToStringLen(a.y) + 1;
}

template <class coord>
inline void xToString(char* s, point2d<coord> a) { 
  int l = xToStringLen(a.x);
  xToString(s, a.x);
  s[l] = ' ';
  xToString(s+l+1, a.y);
}

template <class coord>
inline int xToStringLen(point3d<coord> a) { 
  return xToStringLen(a.x) + xToStringLen(a.y) + xToStringLen(a.z) + 2;
}

template <class coord>
inline void xToString(char* s, point3d<coord> a) { 
  int lx = xToStringLen(a.x);
  int ly = xToStringLen(a.y);
  xToString(s, a.x);
  s[lx] = ' ';
  xToString(s+lx+1, a.y);
  s[lx+ly+1] = ' ';
  xToString(s+lx+ly+2, a.z);
}

/*
inline int xToStringLen(triangle a) { 
  return xToStringLen(a.C[0]) + xToStringLen(a.C[1]) + xToStringLen(a.C[2]) + 2;
}

inline void xToString(char* s, triangle a) { 
  int lx = xToStringLen(a.C[0]);
  int ly = xToStringLen(a.C[1]);
  xToString(s, a.C[0]);
  s[lx] = ' ';
  xToString(s+lx+1, a.C[1]);
  s[lx+ly+1] = ' ';
  xToString(s+lx+ly+2, a.C[2]);
}
*/
namespace benchIO {
  using namespace std;

  string HeaderPoint2d = "pbbs_sequencePoint2d";
  string HeaderPoint3d = "pbbs_sequencePoint3d";
  string HeaderTriangles = "pbbs_triangles";

  template <class pointT, class intV = DefaultIntV>
  int writePointsToFile(sequence<pointT> const& Points, char const *fileName) {
    string header = (pointT::dim == 2) ? HeaderPoint2d : HeaderPoint3d;
    return writeSeqToFile(header, Points, fileName);
  }

  /*
  template <class pointT>
  sequence<pointT> parsePoints(range<char**> strings) {
    using coordT = typename pointT::coordT;
    int d = pointT::dim;
    auto n = strings.size();
    sequence<coordT> a(d*n, [&] (size_t i) {
      return atof(strings[i]);
    });
    sequence<pointT> P(n, [&] (size_t i) {
      return pointT(a.begin()+(d*i));
    });
    return P;
  }
*/
  
  template <class pointT>
  sequence<pointT> readPointsFromFile(char const* fileName) {
    using coordT = typename pointT::coordT;
    int d = pointT::dim;
    sequence<char> S = pbbs::char_seq_from_file(fileName);
    sequence<char*> W = pbbs::tokenize(S, pbbs::is_space);
    string header = (string) W[0];
    if (W.size() == 0 || (string)W[0] != (d == 2 ? HeaderPoint2d : HeaderPoint3d)) {
      cout << "readPointsFromFile wrong file type" << endl;
      abort();
    }
    long n = W.size()-1;
    sequence<coordT> A(n, [&] (size_t i) {return atof(W[i+1]);});
    sequence<pointT> P(n/d, [&] (size_t i) {
      return pointT(A.begin()+(d*i));
    });
    return P;
  }

  /*
  template <class intV = DefaultIntV>
  triangles<point2d> readTrianglesFromFileNodeEle(char* fname) {
    string nfilename(fname);
    _seq<char> S = readStringFromFile((char*)nfilename.append(".node").c_str());
    words W = stringToWords(S.A, S.n);
    triangles<point2d> Tr;
    Tr.numPoints = atol(W.Strings[0]);
    if (W.m < 4*Tr.numPoints + 4) {
      cout << "readStringFromFileNodeEle inconsistent length" << endl;
      abort();
    }

    Tr.P = newA(point2d, Tr.numPoints);
    for(intT i=0; i < Tr.numPoints; i++) 
      Tr.P[i] = point2d(atof(W.Strings[4*i+5]), atof(W.Strings[4*i+6]));

    string efilename(fname);
    _seq<char> SN = readStringFromFile((char*)efilename.append(".ele").c_str());
    words WE = stringToWords(SN.A, SN.n);
    Tr.numTriangles = atol(WE.Strings[0]);
    if (WE.m < 4*Tr.numTriangles + 3) {
      cout << "readStringFromFileNodeEle inconsistent length" << endl;
      abort();
    }

    Tr.T = newA(triangle, Tr.numTriangles);
    for (long i=0; i < Tr.numTriangles; i++)
      for (int j=0; j < 3; j++)
	Tr.T[i].C[j] = atol(WE.Strings[4*i + 4 + j]);

    return Tr;
  }

  template <class pointT>
    triangles<pointT> readTrianglesFromFile(char* fname, intV offset) {
    int d = pointT::dim;
    _seq<char> S = readStringFromFile(fname);
    words W = stringToWords(S.A, S.n);
    if (W.Strings[0] != HeaderTriangles) {
      cout << "readTrianglesFromFile wrong file type" << endl;
      abort();
    }

    int headerSize = 3;
    triangles<pointT> Tr;
    Tr.numPoints = atol(W.Strings[1]);
    Tr.numTriangles = atol(W.Strings[2]);
    if (W.m != headerSize + 3 * Tr.numTriangles + d * Tr.numPoints) {
      cout << "readTrianglesFromFile inconsistent length" << endl;
      abort();
    }

    Tr.P = newA(pointT, Tr.numPoints);
    parsePoints(W.Strings + headerSize, Tr.P, Tr.numPoints);

    Tr.T = newA(triangle, Tr.numTriangles);
    char** Triangles = W.Strings + headerSize + d * Tr.numPoints;
    for (long i=0; i < Tr.numTriangles; i++)
      for (int j=0; j < 3; j++)
	Tr.T[i].C[j] = atol(Triangles[3*i + j])-offset;
    return Tr;
  }

  template <class pointT>
  int writeTrianglesToFile(triangles<pointT> Tr, char* fileName) {
    ofstream file (fileName, ios::binary);
    if (!file.is_open()) {
      std::cout << "Unable to open file: " << fileName << std::endl;
      return 1;
    }
    file << HeaderTriangles << endl;
    file << Tr.numPoints << endl; 
    file << Tr.numTriangles << endl; 
    writeArrayToStream(file, Tr.P, Tr.numPoints);
    writeArrayToStream(file, Tr.T, Tr.numTriangles);
    file.close();
    return 0;
  }
  */
};
