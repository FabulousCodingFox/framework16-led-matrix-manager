# Framework16 LED Matrix Manager

# Developement

To initiate a development shell with the necessary dependencies and environment configurations, execute the following command:

```bash
nix develop
```

This command sets up a Nix shell that provides all required dependencies without modifying the host system.

# Building

The following sections outline the build process for different target platforms.

## Linux

For Linux-based systems, the package is compiled for a standard **GNU/Linux x86-64 environment**. Use the following command to build the package:

```bash
nix build .#packages.x86_64-linux.default
```

## Windows

**Note**: The Windows build process is currently non-functional due to the lack of cross-compilation support in `nixpkgs.qt6`. Attempting to build for Windows will result in an error.

For reference, the following command would be used if cross-compilation were supported:

```bash
nix build .#packages.x86_64-windows.default
```

Future updates to `nixpkgs.qt6` may enable cross-compilation, at which point this command can be revisited.
