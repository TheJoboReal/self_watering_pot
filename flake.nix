{
  description = "ESP-IDF dev shell";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

    esp-dev.url = "github:mirrexagon/nixpkgs-esp-dev";
  };

  outputs = { self, nixpkgs, esp-dev }:
  let
    system = "x86_64-linux";
  in {
    devShells.${system}.default =
      esp-dev.devShells.${system}.esp-idf-full;
  };
}
