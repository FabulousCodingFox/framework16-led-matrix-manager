{
  description = "A gui tool to control the Framework 16 led displays";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  };

  outputs =
    { self, nixpkgs }:
    let
      # Only build for x86_64-linux and x86_64-windows, as these are the only platforms officially supported by the Framework 16
      systems = [
        "x86_64-linux" # Native Linux x86_64
        "x86_64-windows" # Windows 64-bit (MinGW)
      ];

      # Common dependencies for all builds and devShell
      commonDeps =
        pkgs: with pkgs; [
          cmake
          gdb
          clang
          ninja
          zip
          unzip
          curl
          git
          libusb1
          hidapi
          spdlog
          clang-tools
          portaudio
          fftw
          alsa-lib
          pipewire

          # Wrapper
          makeWrapper
          bashInteractive

          # QT6
          qt6.full
          qt6.qtbase
          qt6.wrapQtAppsHook
        ];

      # Generate packages for each system
      packagesBySystem = nixpkgs.lib.genAttrs systems (
        system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.stdenv.mkDerivation {
            name = "framework16-led-info-display";
            src = ./.;

            nativeBuildInputs = [
              pkgs.cmake
              pkgs.ninja
              pkgs.pkg-config
            ];

            buildInputs = commonDeps pkgs;

            configurePhase = ''
              cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
            '';

            buildPhase = ''
              cmake --build build
            '';

            installPhase = ''
              mkdir -p $out/bin
              cp -r build/* $out/bin/
            '';
          };
        }
      );
    in
    {
      packages = packagesBySystem;

      # DevShell for interactive development
      devShells.x86_64-linux.default = nixpkgs.legacyPackages.x86_64-linux.mkShell {
        buildInputs = commonDeps nixpkgs.legacyPackages.x86_64-linux;
        nativeBuildInputs = [
          nixpkgs.legacyPackages.x86_64-linux.pkg-config
        ];
        shellHook = ''
          export QT_QPA_PLATFORM=wayland
          bashdir=$(mktemp -d)
          makeWrapper "$(type -p bash)" "$bashdir/bash" "''${qtWrapperArgs[@]}"
          exec "$bashdir/bash"
        '';
      };
    };
}
