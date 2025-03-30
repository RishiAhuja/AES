# Axon: Advanced AES Encryption CLI Tool

A high-performance, secure file encryption and decryption tool using AES-128.

![Axon Logo](assets/logo.jpg)

## Table of Contents

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Installation](#installation)
  - [Linux](#linux)
  - [macOS](#macos)
  - [Windows](#windows)
  - [From Source](#from-source)
- [Requirements](#requirements)
- [Usage](#usage)
  - [Basic Commands](#basic-commands)
  - [Examples](#examples)
- [Security Considerations](#security-considerations)
- [Development](#development)
  - [Building from Source](#building-from-source)
  - [Running Tests](#running-tests)
- [Troubleshooting](#troubleshooting)

## Overview

Axon is a command-line tool that provides strong encryption and decryption capabilities using the Advanced Encryption Standard (AES-128) with Cipher Block Chaining (CBC) mode. It's designed for secure file protection with a focus on performance, security, and ease of use.

## Features

- **AES-128 Encryption**: Industry-standard symmetric encryption algorithm
- **CBC Mode Implementation**: Enhanced security through block chaining
- **Secure Password Handling**: Strong password validation and key derivation
- **High Performance**: Optimized C implementation for fast encryption/decryption
- **Command Line Interface**: Simple, scriptable interface for automation
- **Cross-Platform Support**: Works on Linux, macOS, and Windows
- **File Chunking**: Efficiently handles files of any size by processing in chunks
- **Error Recovery**: Robust error handling and reporting

## Architecture

### Component Architecture

```mermaid
flowchart TD
    subgraph UI["User Interface Layer"]
        main["main.c - CLI Interface"]
    end

    subgraph IO["Input/Output Layer"]
        chunked_file["chunked_file.c - File Processing"]
    end

    subgraph CE["Crypto Engine"]
        encryptor["encryptor.c - Encryption Logic"]
        decryptor["decryptor.c - Decryption Logic"]
    end

    subgraph AESCORE["AES Core Operations"]
        confusion["confusion.c - SubBytes"]
        diffusion["diffusion.c - ShiftRows, MixColumns"]
        key["key_expansion.c - Key Scheduling"]
    end

    subgraph UTILS["Utility Functions"]
        memory["memory.c - Memory Management"]
        conversion["conversion.c - Data Conversion"]
        config["config.h - Configuration"]
        error["errors.h - Error Handling"]
    end

    UI --> CE
    UI --> IO
    CE --> AESCORE
    CE --> UTILS
    IO --> UTILS
    AESCORE --> UTILS
```

### Data Flow

```mermaid
flowchart LR
    subgraph input["Input Processing"]
        in_file["Source File"] --> chunker["file_chunker()"]
        password["Password"] --> validator["validate_password()"]
        chunker --> states["Chunked States"]
        validator --> key["Derived Key"]
    end

    subgraph crypto["AES Processing"]
        states --> encryptor["chain_encryptor()"]
        key --> expander["expand_key()"]
        expander --> round_keys["Expanded Round Keys"]
        round_keys --> aes["AES Round Operations"]
        encryptor --> aes
        aes --> encrypted["Encrypted Chunks"]
    end

    subgraph output["Output Processing"]
        encrypted --> writer["chunk_writer()"]
        writer --> out_file["Destination File"]
    end

    classDef process fill:#f9f,stroke:#333,stroke-width:2px;
    classDef data fill:#bbf,stroke:#33f,stroke-width:2px;
    classDef file fill:#bfb,stroke:#3f3,stroke-width:2px;
    
    class chunker,validator,encryptor,expander,aes,writer process;
    class states,key,round_keys,encrypted data;
    class in_file,out_file file;
```

### AES Round Structure

```mermaid
stateDiagram-v2
    [*] --> Initial: Input State
    Initial --> Round1: AddRoundKey (Round 0 Key)
    
    state Rounds {
        Round1 --> Round2: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round2 --> Round3: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round3 --> Round4: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round4 --> Round5: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round5 --> Round6: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round6 --> Round7: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round7 --> Round8: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round8 --> Round9: SubBytes → ShiftRows → MixColumns → AddRoundKey
        Round9 --> Final: SubBytes → ShiftRows → MixColumns → AddRoundKey
    }
    
    Final --> Output: SubBytes → ShiftRows → AddRoundKey (Final)
    Output --> [*]: Encrypted State
```


### Component Description

1. **User Interface Layer**
   - Handles command-line arguments
   - Validates inputs
   - Coordinates the encryption/decryption process
   - Reports results and errors

2. **Crypto Engine**
   - Implements the AES algorithm
   - Manages encryption and decryption workflows
   - Handles the CBC mode implementation

3. **AES Core Operations**
   - **Confusion**: SubBytes operation using S-Box substitution
   - **Diffusion**: ShiftRows and MixColumns operations
   - **Key Expansion**: Derives round keys from the master key

4. **I/O and Utilities**
   - File chunking and processing
   - Memory management
   - Error handling
   - Conversion utilities (hex/binary)

## Installation

### Requirements

- **Build Requirements**:
  - C Compiler (GCC 7+ or Clang 5+)
  - CMake (3.10+)
  - Make or Ninja build system
  - Git (for cloning)

- **Runtime Requirements**:
  - Linux: glibc 2.17+ 
  - macOS: 10.13+ (High Sierra or newer)
  - Windows: Windows 7 SP1 or newer

### Linux

#### Using the Install Script

```bash
git clone https://github.com/RishiAhuja/axon.git
cd axon
chmod +x install.sh
./install.sh
```

#### Manual Installation

```bash
# Clone the repository
git clone https://github.com/RishiAhuja/axon.git
cd axon

# Install using the script
chmod +x install.sh
./install.sh

# Or build manually
mkdir -p build && cd build
cmake ..
make
sudo make install
```

#### Debian/Ubuntu Package

```bash
# Download the latest .deb package
wget https://github.com/RishiAhuja/axon/releases/download/v1.0.0/axon_1.0.0_amd64.deb

# Install the package
sudo dpkg -i axon_1.0.0_amd64.deb
sudo apt-get install -f  # Install any missing dependencies
```

### macOS

#### Using Homebrew

```bash
# Add the tap repository
brew tap RishiAhuja/axon

# Install axon
brew install axon
```

#### Manual Installation

```bash
git clone https://github.com/RishiAhuja/axon.git
cd axon
mkdir -p build && cd build
cmake ..
make
sudo make install
```

### Windows

#### Using the Installer

1. Download the latest installer from the [releases page](https://github.com/username/axon/releases)
2. Run `AxonInstaller.exe`
3. Follow the installation wizard

#### Using Chocolatey

```powershell
choco install axon
```

### From Source

For all platforms, you can build and install from source:

```bash
# Clone the repository
git clone https://github.com/RishiAhuja/axon.git
cd axon

# Create build directory
mkdir -p build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Install (may require admin/sudo)
cmake --install .
```

## Usage

### Basic Commands

```bash
# Encrypt a file
axon <source_file> <destination_file> <key> e

# Decrypt a file
axon <source_file> <destination_file> <key> d
```

### Examples

```bash
# Encrypt a text file
axon confidential.txt confidential.enc "my-secure-password" e

# Decrypt the file
axon confidential.enc decrypted.txt "my-secure-password" d

# Process a binary file
axon image.jpg image.enc "password123" e
```

## Security Considerations

- **Password Strength**: Use strong, unique passwords (12+ characters with a mix of types)
- **File Handling**: Securely delete sensitive plaintext files after encryption
- **Key Management**: Never store encryption keys in plaintext or share them insecurely
- **Memory Safety**: Axon implements secure memory handling to minimize exposure of sensitive data

## Development

### Building from Source

```bash
git clone https://github.com/RishiAhuja/axon.git
cd axon
mkdir -p build && cd build
cmake ..
make
sudo make install
```

### Running Tests

```bash
# In the build directory
ctest
```

### Project Structure

```
axon/                          # Root project directory
├── CMakeLists.txt             # Main build configuration
├── LICENSE                    # License file
├── README.md                  # This documentation
├── INSTALL.md                 # Detailed installation instructions
├── install.sh                 # Linux/macOS installation script
├── include/                   # Header files
│   ├── common/                # Common definitions
│   │   ├── config.h           # Configuration constants
│   │   └── errors.h           # Error codes and messages
│   ├── crypto/                # Cryptography headers
│   │   ├── aes.h              # AES algorithm interface
│   │   ├── confusion.h        # Confusion operations (SubBytes)
│   │   ├── diffusion.h        # Diffusion operations (ShiftRows, MixColumns)
│   │   ├── encryptor.h        # Encryption interface
│   │   ├── decryptor.h        # Decryption interface
│   │   └── key_expansion.h    # Key handling
│   └── utils/                 # Utility headers
│       ├── conversion.h       # Format conversion utilities
│       ├── memory.h           # Memory management utilities
│       └── file.h             # File handling utilities
├── lib/                       # Library source files
│   ├── crypto/                # Cryptography implementation
│   │   ├── aes.c              # AES algorithm implementation
│   │   ├── confusion.c        # Confusion operations implementation
│   │   ├── diffusion.c        # Diffusion operations implementation
│   │   ├── encryptor.c        # Encryption implementation
│   │   ├── decryptor.c        # Decryption implementation
│   │   └── key_expansion.c    # Key handling implementation
│   └── utils/                 # Utility implementations
│       ├── conversion.c       # Format conversion utilities
│       ├── memory.c           # Memory management utilities
│       └── file.c             # File handling utilities
├── src/                       # Application source
│   └── main.c                 # Main application entry point
├── man/                       # Manual pages
│   └── axon.1                 # Man page for the axon command
├── tests/                     # Test suite
│   ├── CMakeLists.txt         # Test build configuration
│   ├── test_aes.c             # AES algorithm tests
│   ├── test_encryption.c      # Encryption tests
│   └── test_decryption.c      # Decryption tests
├── examples/                  # Example files and scripts
│   ├── example.txt            # Sample plaintext
│   └── example.sh             # Sample usage script
└── packaging/                 # Packaging scripts
    ├── windows/               # Windows packaging
    │   └── axon.nsi           # NSIS installer script
    ├── macos/                 # macOS packaging
    │   └── axon.rb            # Homebrew formula
    └── linux/                 # Linux packaging
        └── debian/            # Debian packaging
            └── control        # Debian package control file
```

## Troubleshooting

### Common Issues

1. **"Command not found" error**
   - Make sure the installation directory is in your PATH
   - Try running with the full path to the executable

2. **"Invalid password" error**
   - Ensure the password meets the minimum requirements
   - Check for typos or encoding issues

3. **Build failures**
   - Verify you have all required build dependencies
   - Check the CMake output for specific errors

4. **Decryption errors**
   - Verify you're using the exact same password used for encryption
   - Ensure the file wasn't corrupted during transfer

### Getting Help

For additional help:
- Submit an issue on [GitHub](https://github.com/RishiAhuja/axon/issues)
- Check the detailed [documentation](https://github.com/RishiAhuja/axon/wiki)

