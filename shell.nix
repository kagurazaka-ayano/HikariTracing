with import <nixpkgs> {}; let
in
  pkgs.mkShell.override {stdenv = clangStdenv;} {
    packages = [
      pkgs.cmake
      vulkan-tools
      vulkan-loader
      pkg-config
      xorg.libX11
      xorg.libXrandr
      xorg.libXinerama
      xorg.libXcursor
      xorg.libXi
      vulkan-headers
      cmake
    ];
    NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [stdenv.cc.cc cmake ninja];
    NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
  }
