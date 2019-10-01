{ pkgs   ? import <nixpkgs> {},
  stdenv ? pkgs.stdenv,
  makeWrapper ? pkgs.makeWrapper,
  gcc ? pkgs.gcc7,
  which ? pkgs.which,
  jemalloc ? pkgs.jemalloc,
  useJemalloc ? false,
  dfltSrc ? ./.,
  pbbslib ? import ../pbbslib/default.nix {}
}:

stdenv.mkDerivation rec {
  name = "pbbslib";
  
  src = dfltSrc;

  buildInputs =
    [ makeWrapper gcc which ]
    ++ (if useJemalloc then [ jemalloc ] else []);
  
  buildPhase =
    ''
    rm -f pbbslib
    ln -s ${pbbslib} pbbslib

    make -j -C testData/geometryData all
    make -j -C testData/graphData all
    '';

  installPhase =
    ''
    mkdir -p $out/

    mkdir -p $out/common/
    cp common/*.h $out/common

    mkdir -p $out/testData/geometryData
    make -C testData/geometryData install INSTALL_FOLDER="$out/testData/geometryData"
    
    mkdir -p $out/testData/
    mkdir -p $out/testData/graphData
    make -C testData/graphData install INSTALL_FOLDER="$out/testData/graphData"

    BENCHMARKS=$out/benchmarkSrc/b/
    mkdir -p $BENCHMARKS
    (cd convexHull/quickHull; cp *.h *.C $BENCHMARKS)

    ln -s ${pbbslib} $out/pbbslib
    '';
}   
