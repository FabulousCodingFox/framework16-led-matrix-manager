{
  description = "A GUI tool to control the Framework 16 LED displays";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      package-x86_64-linux = import ./packages/x86_64-linux.nix;
      package-x86_64-windows = import ./packages/x86_64-windows.nix;

      current_system = "x86_64-linux";

      system_packages = nixpkgs.legacyPackages.x86_64-linux;

      package_name = "framework16-led-matrix-manager";
      package_version = "0.0.1";
      package_meta = with nixpkgs.lib; {
        description = "GUI tool for Framework16 LED displays";
        license = licenses.boost;
      };
    in
    {
      packages = {
        x86_64-linux.default = package-x86_64-linux {
          nixpkgs = nixpkgs;
          current_system = current_system;
          package_name = package_name;
          package_version = package_version;
          package_meta = package_meta;
        };
        x86_64-windows.default = package-x86_64-windows {
          nixpkgs = nixpkgs;
          current_system = current_system;
          package_name = package_name;
          package_version = package_version;
          package_meta = package_meta;
        };
      };

      apps = {
        x86_64-linux.default = {
          type = "app";
          program = "${self.packages.x86_64-linux.default}/bin/linux-release/framework16-led-matrix-manager";
        };
        x86_64-windows.default = {
          type = "app";
          program = "${self.packages.x86_64-windows.default}/bin/windows-release/framework16-led-matrix-manager.exe";
        };
      };

      devShells.x86_64-linux.default = system_packages.mkShell {
        buildInputs = with system_packages; [
          spdlog
          libusb1
          qt6.qttools
          qt6.qtbase
          qt6.wrapQtAppsHook

          clang
          cmake
          ninja
          pkg-config

          clang-tools
          gdb
          zip
          unzip
          curl
          git
          makeWrapper
          bashInteractive
        ];
      };
    };
}
