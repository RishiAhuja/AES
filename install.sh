#!/bin/bash
# Axon: AES Encryption Tool - Installer Script

# Print colored text
print_color() {
    if [ -t 1 ]; then
        echo -e "\033[1;$2m$1\033[0m"
    else
        echo "$1"
    fi
}

# Print header
print_color "==============================================" 34
print_color "     Axon: AES Encryption Tool Installer      " 34
print_color "==============================================" 34
echo ""

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macOS"
elif [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" ]]; then
    OS="Windows"
else
    OS="Unknown"
fi

print_color "Detected OS: $OS" 33

# Check for dependencies
print_color "Checking dependencies..." 33

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

MISSING_DEPS=0

# Check for cmake
if ! command_exists cmake; then
    print_color "Error: cmake not found. Please install cmake." 31
    MISSING_DEPS=1
fi

# Check for a C compiler
if ! command_exists gcc && ! command_exists clang; then
    print_color "Error: No C compiler found. Please install gcc or clang." 31
    MISSING_DEPS=1
fi

# Check for make
if ! command_exists make && ! command_exists ninja; then
    print_color "Error: No build system found. Please install make or ninja." 31
    MISSING_DEPS=1
fi

if [ $MISSING_DEPS -eq 1 ]; then
    print_color "Please install the missing dependencies and run this script again." 31
    exit 1
fi

# Create build directory
print_color "Creating build directory..." 33
mkdir -p build
cd build || {
    print_color "Error: Failed to create or access the build directory." 31
    exit 1
}

# Configure with CMake
print_color "Configuring with CMake..." 33
cmake .. || {
    print_color "Error: CMake configuration failed." 31
    exit 1
}

# Build
print_color "Building Axon..." 33
cmake --build . || {
    print_color "Error: Build failed." 31
    exit 1
}

# Install
print_color "Installing Axon..." 33
if [ "$OS" == "Windows" ]; then
    cmake --install . || {
        print_color "Error: Installation failed. Try running as administrator." 31
        exit 1
    }
else
    sudo cmake --install . || {
        print_color "Error: Installation failed. Try running with sudo." 31
        exit 1
    }
fi

# Installation complete
print_color "==============================================" 32
print_color "     Axon has been successfully installed!    " 32
print_color "==============================================" 32
echo ""
print_color "Usage:" 36
print_color "  axon <source_file> <destination_file> <key> e|d" 36
echo ""
print_color "Examples:" 36
print_color "  Encrypt: axon secret.txt secret.enc \"mypassword\" e" 36
print_color "  Decrypt: axon secret.enc secret.txt \"mypassword\" d" 36
echo ""
print_color "For more information, visit:" 36
print_color "  https://github.com/RishiAhuja/axon" 36
echo ""

exit 0