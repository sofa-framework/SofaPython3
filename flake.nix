{
  description = "Real-time multi-physics simulation with an emphasis on medical simulation.";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    #nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    # ref. https://github.com/NixOS/nixpkgs/pull/348549
    nixpkgs.url = "github:nim65s/nixpkgs/qt6-libqglviewer";
    sofa = {
      url = "github:nim65s/sofa"; # update this after PR
      inputs = {
        flake-parts.follows = "flake-parts";
        nixpkgs.follows = "nixpkgs";
      };
    };
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = inputs.nixpkgs.lib.systems.flakeExposed;
      perSystem =
        {
          pkgs,
          self',
          system,
          ...
        }:
        {
          apps.default = {
            type = "app";
            program =
              pkgs.runCommand "runSofa"
                {
                  nativeBuildInputs = [ pkgs.makeBinaryWrapper ];
                  meta.mainProgram = "runSofa";
                }
                ''
                  makeBinaryWrapper ${pkgs.lib.getExe self'.packages.sofa} $out/bin/runSofa \
                    --set SOFA_PLUGIN_PATH ${self'.packages.sofa-python3} \
                    --set SOFA_ROOT ${self'.packages.sofa} \
                    --add-flags "-l SofaPython3"
                '';
          };
          devShells.default = pkgs.mkShell { inputsFrom = [ self'.packages.default ]; };
          packages = {
            inherit (inputs.sofa.packages.${system}) sofa;
            default = self'.packages.sofa-python3;
            sofa-python3 = pkgs.callPackage ./package.nix { inherit (self'.packages) sofa; };
          };
        };
    };
}
