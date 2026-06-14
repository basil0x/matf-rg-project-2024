#!/bin/bash
sudo dnf install -y \
  pkgconfig \
  gcc \
  g++ \
  clang-tools-extra \
  cmake \
  git \
  make \
  automake \
  wayland-devel \
  libxkbcommon-devel \
  libX11-devel \
  libXrandr-devel \
  libXi-devel \
  libXcursor-devel \
  libXinerama-devel \
  mesa-libGL-devel \
  mesa-libGLU-devel \
  glx-utils \
  doxygen \
  graphviz \
  assimp-devel \
  python3-pip \
  python3

python3 -m venv .venv
source ".venv/bin/activate" && pip3 install libclang
