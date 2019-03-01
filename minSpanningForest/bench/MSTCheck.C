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

#include <iostream>
#include <algorithm>
#include <cstring>
#include "parallel.h"
#include "IO.h"
#include "graph.h"
#include "graphIO.h"
#include "parse_command_line.h"
#include "MST.h"

using namespace std;

int main(int argc, char* argv[]) { 
  commandLine P(argc,argv,"<inFile> <outfile>");
  pair<char*,char*> fnames = P.IOFileNames();
  char* iFile = fnames.first;
  char* oFile = fnames.second;
  wghEdgeArray<intT> In = readWghEdgeArrayFromFile<intT>(iFile);
  pbbs::sequence<intT> Out = readIntArrayFromFile<intT>(oFile);
  intT n = Out.size();
  intT in_m = In.m;
  //check num edges
  pair<intT*,intT> serialMST = mst(In);
  if (n != serialMST.second) {
    cout << "Wrong edge count: MST has " << serialMST.second
	 << " edges but algorithm returned " << n << " edges\n";
    return (1);
    }
  
  //check for cycles
  sequence<bool> flags(in_m, false);
  parallel_for(0, n, [&] (size_t i) {flags[Out[i]] = true;});
  
  pbbs::sequence<wghEdge<intT>> E = pbbs::pack(In.E, flags);
  wghEdgeArray<intT> EA(std::move(E), In.n);
  
  pair<intT*,intT> check = mst(EA);
  if (n != check.second){
    cout << "Result is not a spanning forest : it has a cycle" << endl;
    return (1);
  }
  
  // check total weight
  double total1 = pbbs::reduce(pbbs::delayed_seq<double>(n, [&] (size_t i) {
	return In[Out[i]].weight;}), pbbs::addm<double>());

  double total2 = pbbs::reduce(pbbs::delayed_seq<double>(n, [&] (size_t i) {
	return In[serialMST.first[i]].weight;}), pbbs::addm<double>());

  if(total1 != total2) {
    cout << "MST has a weight of " << total1 <<
      " but should have a weight of " << total2 << endl;
    return (1);
  }

  return 0;
}