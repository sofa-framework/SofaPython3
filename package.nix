{
  cmake,
  lib,
  python3Packages,
  qt6Packages,
  sofa,
  stdenv,
}:

stdenv.mkDerivation {
  pname = "sofa-python3";
  version = "24.06";

  src = lib.fileset.toSource {
    root = ./.;
    fileset = lib.fileset.unions [
      ./bindings
      ./CMake
      ./CMakeLists.txt
      ./constants
      ./docs
      ./examples
      ./Plugin
      ./SofaPython3Config.cmake.in
      ./splib
      ./Testing
    ];
  };

  nativeBuildInputs = [
    cmake
    qt6Packages.libqglviewer
    qt6Packages.wrapQtAppsHook
  ];
  propagatedBuildInputs = [
    python3Packages.pybind11
    python3Packages.scipy
    sofa
  ];

  # help locate FindQGLViewer.cmake
  cmakeFlags = [
    "-DCMAKE_MODULE_PATH=${sofa.src}/cmake/Modules"
  ];

  meta = {
    description = "python plugin for Sofa offering a pythonic interface and python3 support";
    homepage = "https://github.com/sofa-framework/SofaPython3";
    license = lib.licenses.lgpl21Only;
    maintainers = with lib.maintainers; [ nim65s ];
    platforms = lib.platforms.unix ++ lib.platforms.windows;
  };
}
