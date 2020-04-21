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

#include <math.h>
#include <vector>
#include "pbbslib/parallel.h"
#include "pbbslib/parse_command_line.h"
#include "common/graph.h"
#include "common/graphIO.h"
#include "common/graphUtils.h"

using namespace benchIO;
using namespace dataGen;
using namespace std;

template <class intV>
edgeArray<intV> parallelPaths(size_t nb_phases,
                              size_t nb_paths_per_phase,
                              size_t nb_edges_per_path) {
  vector<edge<intV>> edges;
  auto P = nb_paths_per_phase;
  auto L = nb_edges_per_path;
  for (size_t r = 0; r != nb_phases; r++) {
    auto s = r * (P * L + 1);
    auto e = s + P * L + 1;
    for (size_t k = 0; k != P; k++) {
      auto ps = s + 1 + k * L;
      auto pe = ps + L - 1;
      edges.push_back(edge<intV>(s, ps));
      for (size_t p = 0; p != L - 1; p++) {
        edges.push_back(edge<intV>(ps + p, ps + p + 1));
      }
      edges.push_back(edge<intV>(pe, e));
    }
  }
  auto n = nb_phases * nb_paths_per_phase * nb_edges_per_path + nb_phases + 1;
  pbbs::sequence<edge<intV>> E(edges.size(), [&] (size_t i) { return edges[i]; });
  return edgeArray<intV>(std::move(E), n, n);
}

int main(int argc, char* argv[]) {
  commandLine P(argc,argv,"[-j] [-o] n <outFile>");
  pair<int,char*> in = P.sizeAndFileName();
  size_t n = in.first;
  char* fname = in.second;
  bool ordered = P.getOption("-o");
  bool adjArray = P.getOption("-j");
  size_t parallel_paths_nb_phases = 3;
  size_t parallel_paths_nb_paths_per_phase = 10;
  size_t parallel_paths_nb_edges_per_path = 10;
  auto nb_phases = n / parallel_paths_nb_phases / parallel_paths_nb_paths_per_phase;
  auto nb_edges_per_path = std::max((size_t)1, n);
  edgeArray<size_t> EA = parallelPaths<size_t>(parallel_paths_nb_phases, parallel_paths_nb_paths_per_phase, nb_edges_per_path);
  writeGraphFromEdges(EA, fname, adjArray, ordered);
}
