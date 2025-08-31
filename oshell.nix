with import <nixpkgs> {};

stdenv.mkDerivation {
  name = "myenv";
  buildInputs = with pkgs; [
	pkgs.SDL2
    pkgs.SDL2_ttf
    pkgs.SDL2_net
    pkgs.SDL2_gfx
    pkgs.SDL2_image
    pkgs.SDL2_sound
    pkgs.SDL2_mixer
    pkgs.raylib
    pkgs.libGL
    pkgs.libGLU
    pkgs.openal
    pkgs.libglvnd
    pkgs.mesa
    pkgs.libGL.dev
    pkgs.libglvnd.dev
    pkgs.glfw
    pkgs.glm
    pkgs.freeglut
    pkgs.glew
    pkgs.xorg.libX11
    pkgs.xorg.libX11.dev
    pkgs.xorg.libX11
    pkgs.xorg.libXrandr
    pkgs.xorg.libXcursor
    pkgs.xorg.libXinerama
    pkgs.xorg.libXi
    pkgs.xorg.libXxf86vm
  ];

}
