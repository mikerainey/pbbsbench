# To generate symlinks for all attributes, including test-data gen, run
#   $ nix-build -A all
# and look for symlinks named `result` and `result-testData`.

{ pkgs   ? import <nixpkgs> {},
  stdenv ? pkgs.stdenv,
  makeWrapper ? pkgs.makeWrapper,
  gcc ? pkgs.gcc7,
  which ? pkgs.which,
  jemalloc ? pkgs.jemalloc, # use jemalloc, unless this parameter equals null
  pbbslib ? import ../pbbslib/default.nix { gcc = gcc; jemalloc = jemalloc; },
  dfltSrc ? ./.
}:

stdenv.mkDerivation rec {
  name = "pbbsbench";
  
  src = dfltSrc;

  buildInputs =
    [ makeWrapper gcc which ]
    ++ (if jemalloc == null then [] else [ jemalloc ]);

  enableParallelBuilding = true;
  
  buildPhase =
    let jemallocCfg = 
          if jemalloc == null then
            ""
          else
            "export PATH=${jemalloc}/bin:$PATH";
    in
    ''
    ${jemallocCfg}

    rm -f pbbslib
    ln -s ${pbbslib} pbbslib

    make -j $NIX_BUILD_CORES -C testData/geometryData all
    make -j $NIX_BUILD_CORES -C testData/graphData all
    make -j $NIX_BUILD_CORES -C testData/sequenceData all
    '';

  outputs = [ "out" "testData" ];

  installPhase =
    ''
    mkdir -p $out/common/
    cp common/*.h $out/common

    ln -s ${pbbslib} $out/pbbslib

    # Sources
    cp -r convexHull $out/convexHull
    cp -r breadthFirstSearch $out/breadthFirstSearch

    mkdir -p $testData/geometryData
    make -C testData/geometryData install \
      INSTALL_FOLDER="$testData/geometryData"
    
    mkdir -p $testData/graphData
    make -C testData/graphData install \
      INSTALL_FOLDER="$testData/graphData"

    mkdir -p $testData/sequenceData
    make -C testData/sequenceData install \
      INSTALL_FOLDER="$testData/sequenceData"
    '';
}   
