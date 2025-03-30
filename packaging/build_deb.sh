#!/bin/bash
# Build Debian package

# Ensure build directory exists
mkdir -p build
cd build || exit 1

# Configure and build
cmake ..
make

# Copy files to debian package structure
cp axon ../packaging/debian/usr/local/bin/
cp ../man/axon.1 ../packaging/debian/usr/share/man/man1/
cp ../LICENSE ../packaging/debian/usr/share/doc/axon/copyright

# Build the package
cd ../packaging || exit 1
dpkg-deb --build debian axon_1.0.0_amd64.deb

echo "Debian package created: packaging/axon_1.0.0_amd64.deb"