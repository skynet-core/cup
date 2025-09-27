{
  description = "AdaptiveCpp CPU + CUDA Backend (SYCL implementation)";

  inputs = {
    nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0.2505.810395";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs {
          inherit system;
          config.allowUnfree = true;
        };
        lib = pkgs.lib;
        fs = lib.fileset;
        stdenv = pkgs.gcc15Stdenv;
        src = fs.toSource {
          root = ./.;
          fileset = fs.unions [
            (fs.gitTracked ./.)
            (fs.fromSource ./cmake)
            (fs.fromSource ./debian)
          ];
        };
      in
      {
        packages.default = stdenv.mkDerivation {
          pname = "depot";
          version = "0.1.0";
          inherit src;
          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            gcc15
            patchelf
            dpkg
            rpm
          ];

          buildInputs = with pkgs; [
            cli11
            stdenv.cc.cc.lib
          ];
          cmakeFlags = [
            "-DCMAKE_CXX_STANDARD=23"
            "-DCMAKE_CXX_EXTENSIONS=OFF"
            "-DCMAKE_CXX_STANDARD_REQUIRED=ON"
            "-DDEPOT_STANDALONE_PACKAGE=OFF"
          ];

          __structuredAttrs = true;
        };
        devShells.default =
          pkgs.mkShell.override
            {
              stdenv = pkgs.gcc15Stdenv;
            }
            {
              buildInputs = with pkgs; [
                gcc15
                cmake
                ninja
                patchelf
                dpkg
                rpm
              ];
              shellHook = ''
                cat << EOF | g++ -x c++ -
                #include <iostream>
                #include <filesystem>
                int main() {
                    std::cout << std::filesystem::path{"/"} << " Hello, World"<< std::endl;
                    return 0;
                }
                EOF
                rm a.out
              '';
            };
      }
    );
}
