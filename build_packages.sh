#!/bin/bash
# build_packages.sh - Create distribution packages for all platforms

VERSION="1.0.0"
echo "Building Axon v$VERSION packages..."

# Clean any previous builds
rm -rf dist
mkdir -p dist

# Build for Linux
echo "Building Linux packages..."
# Debian package
mkdir -p build_deb && cd build_deb
cmake ..
make
cd ..
./packaging/build_deb.sh
mv packaging/axon_${VERSION}_amd64.deb dist/

# Build for macOS (if on macOS)
if [[ "$(uname)" == "Darwin" ]]; then
    echo "Building macOS package..."
    mkdir -p build_mac && cd build_mac
    cmake ..
    make
    cd ..
    # Create a simple .tar.gz
    tar -czf dist/axon-${VERSION}-mac.tar.gz build_mac/axon man/axon.1 LICENSE README.md
fi

# Build for Windows (requires cross-compilation or Windows environment)
if command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    echo "Building Windows package..."
    mkdir -p build_win && cd build_win
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw-toolchain.cmake
    make
    cd ..
    # Copy to Windows packaging directory
    cp build_win/axon.exe packaging/windows/
    # Build NSIS installer (if makensis is available)
    if command -v makensis >/dev/null 2>&1; then
        cd packaging/windows
        makensis axon.nsi
        mv AxonInstaller.exe ../../dist/
        cd ../..
    else
        echo "NSIS not found, skipping Windows installer creation"
    fi
fi

echo "All packages built successfully in the 'dist' directory"