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
  name = "pbbslib";
  
  src = dfltSrc;

  buildInputs =
    [ makeWrapper gcc which ]
    ++ (if jemalloc == null then [] else [ jemalloc ]);

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

    make -j -C testData/geometryData all
    make -j -C testData/graphData all
    '';

  outputs = [ "out" "testData" ];

  installPhase =
    ''
    mkdir -p $out/common/
    cp common/*.h $out/common

    ln -s ${pbbslib} $out/pbbslib

    ALL_BENCHMARKS=$out/benchmarks/all/
    mkdir -p $ALL_BENCHMARKS
    (cd convexHull/quickHull; cp *.h *.C $ALL_BENCHMARKS)

    mkdir -p $testData/geometryData
    make -C testData/geometryData install \
      INSTALL_FOLDER="$testData/geometryData"
    
    mkdir -p $testData/graphData
    make -C testData/graphData install \
      INSTALL_FOLDER="$testData/graphData"
    '';
}   
