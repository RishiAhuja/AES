#!/bin/bash
# create_windows_package.sh
# Script to create a Windows package for Axon with a placeholder

# Set version
VERSION="1.0.0"

# Create directory for Windows package
echo "Creating Windows package directory..."
mkdir -p axon-win

# We know you don't have build-win/axon.exe, so we'll create the placeholder
echo "Creating placeholder README..."
echo "This is a placeholder file. Please build the real executable on Windows." > axon-win/README.txt

# Copy documentation files
echo "Copying documentation files..."
cp README.md axon-win/ 2>/dev/null || echo "Warning: Could not copy README.md"
cp LICENSE axon-win/ 2>/dev/null || echo "Warning: Could not copy LICENSE"

# Create installation instructions
echo "Creating installation instructions..."
cat > axon-win/INSTALL.txt << 'EOF'
AXON ENCRYPTION TOOL - WINDOWS INSTALLATION

When you have built the axon.exe executable:

1. Copy axon.exe to a permanent location (e.g., C:\Program Files\Axon)
2. Add that location to your PATH environment variable:
   - Right-click on 'This PC' and select 'Properties'
   - Click on 'Advanced system settings'
   - Click on 'Environment Variables'
   - Under 'System variables', select 'Path' and click 'Edit'
   - Click 'New' and add the path to the Axon directory
   - Click 'OK' on all dialogs

3. Open a new command prompt and type 'axon' to verify installation
EOF

# Create a batch installer
echo "Creating batch installer..."
cat > axon-win/install.bat << 'EOF'
@echo off
echo Axon Windows Installer
echo ---------------------
echo.

:: Check if axon.exe exists in the current directory
if not exist "axon.exe" (
    echo ERROR: axon.exe not found!
    echo This package contains only a placeholder. 
    echo You need to build the actual executable for Windows first.
    echo.
    echo Please see README.txt for more information.
    pause
    exit /b 1
)

:: Create Program Files directory
echo Creating installation directory...
if not exist "%ProgramFiles%\Axon" mkdir "%ProgramFiles%\Axon"

:: Copy executable
echo Copying executable...
copy /Y "axon.exe" "%ProgramFiles%\Axon\"

:: Add to PATH (requires admin privileges)
echo Adding to PATH...
setx /M PATH "%PATH%;%ProgramFiles%\Axon"

echo.
echo Installation complete!
echo You may need to restart your command prompt for the PATH changes to take effect.
pause
EOF

# Create a ZIP file
echo "Creating Windows package ZIP file..."
if command -v zip >/dev/null 2>&1; then
    zip -r axon-${VERSION}-win.zip axon-win
    echo "Created: axon-${VERSION}-win.zip"
else
    # Fallback if zip is not available
    tar -czvf axon-${VERSION}-win.tar.gz axon-win
    echo "Created: axon-${VERSION}-win.tar.gz"
fi

# Clean up
echo "Cleaning up temporary files..."
rm -rf axon-win

echo "Windows package creation completed successfully."