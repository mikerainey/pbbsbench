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
edgeArray<intV> phased(size_t nb_phases,
                       size_t nb_vertices_per_phase,
                       size_t nb_per_phase_at_max_arity,
                       size_t arity_of_vertices_not_at_max_arity=1) {
  auto K = nb_vertices_per_phase;
  vector<edge<intV>> edges;
  size_t n = 0;
  for (size_t r = 0; r != nb_phases-1; r++) {
    for (size_t k = 0; k != K; k++) {
      auto n1 = 1 + r * K + k;
      if (r == 0) {
        edges.push_back(edge<intV>(0, n1));
      }
      auto arity = (k < nb_per_phase_at_max_arity) ? K : arity_of_vertices_not_at_max_arity;
      for (size_t e = 0; e != arity; e++) {
        auto n2 = 1 + (r + 1) * K + ((k + e) % K);
        edges.push_back(edge<intV>(n1, n2));
        n = max(n1, n2);
      }
    }
  }
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
  size_t phased_nb_per_group = 40;
  edgeArray<size_t> EA = phased<size_t>(n, phased_nb_per_group, phased_nb_per_group / 2);
  writeGraphFromEdges(EA, fname, adjArray, ordered);
}
