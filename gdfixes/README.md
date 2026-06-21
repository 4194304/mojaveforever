So you're here I see. While you won't be in for the same amount of pain as I experienced when I made this, you will definitely have quite the experience.

### 1. Make sure you have macports installed
Install it if you haven't already. You will need this

### 2. Install the packages
Install `libcxx`, `libcurl`, and `libssl`

### 3. Compile the shim
Run `clang -dynamiclib -o libgdfix.dylib gdfix.c -framework Foundation -install_name @executable_path/libgdfix.dylib` wherever you have the gdfix.c file

### 4. Place the compiled dylib in the same place as the game binary
The game binary should be in `~/Library/Application Support/Steam/steamapps/common/Geometry Dash/Geometry Dash.app/Contents/MacOS/`. You should see an executable file named Geometry Dash, copy your compiled gdfix dylib into that folder.

### 5. Run the vanilla game with the script
It should launch without any issues, always close the game using Cmd+Q and NEVER open anything that could result in a file dialog being shown, as the game WILL crash.

## Geode

### 1. Force Geode to not use libssl
Go back a folder and go into the Frameworks folder, then run the command `perl -pi -e 's|/usr/lib/libssl.44.dylib|/usr/lib/libxxx.44.dylib|g' Geode.dylib`.

### 2. Launch the game
It should launch without any issues

## Post patch
If you intend to launch the game, I highly recommend that you use my script in this folder. If you choose not to, have fun trying to figure it out, but you will eventually come back here to find the script I have. If you want to make it look like the native Geometry Dash application, create an Automator shortcut to launch the script, and replace the icon (AutomatorApplet.icns) with the Geometry Dash icon file.
