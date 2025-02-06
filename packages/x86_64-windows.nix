{
  nixpkgs,
  current_system,
  package_name,
  package_version,
  package_meta,
  ...
}:
let
  pkgs = import nixpkgs {
    system = current_system;
    crossSystem = nixpkgs.lib.systems.examples.mingwW64;
  };

  crossPkgs = pkgs.pkgsCross.mingwW64;
in
pkgs.stdenv.mkDerivation {
  pname = package_name;
  version = package_version;
  meta = package_meta;
  src = ../.;

  nativeBuildInputs = with crossPkgs.buildPackages; [
    binutils
    clang
    cmake
    ninja
    pkg-config
    qt6.wrapQtAppsHook
  ];

  buildInputs = with crossPkgs; [
    libusb1
    spdlog
    qt6.qttools
    qt6.qtbase
  ];

  configurePhase = ''
    cmake --preset windows-release
  '';

  buildPhase = ''
    cmake --build --preset windows-release-build
  '';

  installPhase = ''
    mkdir -p $out/bin
    cp -r build/* $out/bin/
  '';
}
