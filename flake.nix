{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = {nixpkgs, ...}: let
    systems = ["x86_64-linux" "aarch64-linux"];
    eachSystem = nixpkgs.lib.genAttrs systems;

    pkgsFor = eachSystem (
      system:
        import nixpkgs {inherit system;}
    );

    mkBulidInputs = pkgs:
      with pkgs; [
        clang-tools
        pkg-config
        gnumake
        gcc
        ncurses
      ];
  in {
    devShells = eachSystem (
      system: let
        pkgs = pkgsFor.${system};
        nativeBuildInputs = mkBulidInputs pkgs;
      in
        with pkgs; {
          default = mkShell {
            inherit nativeBuildInputs;
          };
        }
    );

    packages = eachSystem (
      system: let
        pkgs = pkgsFor.${system};
        nativeBuildInputs = mkBulidInputs pkgs;

        fs = nixpkgs.lib.fileset;

        nc_game = pkgs.stdenv.mkDerivation rec {
          pname = "nc_game";
          version = "0.1";
          src = fs.toSource {
            root = ./.;
            fileset = fs.difference ./. (fs.maybeMissing ./build);
          };
          installPhase = ''
            mkdir -p $out/bin
            cp build/${pname} $out/bin
          '';

          inherit nativeBuildInputs;
        };
      in {
        inherit nc_game;
        default = nc_game;
      }
    );
  };
}
