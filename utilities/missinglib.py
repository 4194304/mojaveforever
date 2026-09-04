import struct
import sys
import os

inputfilename = input("Enter the missing system library: ")
def stub_missing_framework(file_path):
    if not os.path.exists(file_path):
        print(f"Error: File '{file_path}' not found.")
        return

    print(f"Fixing framework paths in: {file_path}")
    with open(file_path, "rb") as f:
        data = bytearray(f.read())

    bad_paths = [
        inputfilename.encode('utf-8')
    ]
    
    good_path = b"/usr/lib/libSystem.B.dylib"

    modified = False
    for bad_path in bad_paths:
        offset = 0
        while True:
            offset = data.find(bad_path, offset)
            if offset == -1:
                break
            
            padded_replacement = good_path + b'\x00' * (len(bad_path) - len(good_path))
            data[offset:offset+len(bad_path)] = padded_replacement
            print(f"    SUCCESS: Rerouted missing framework to libSystem at offset {hex(offset)}")
            modified = True
            offset += len(bad_path)

    if modified:
        with open(file_path, "wb") as f:
            f.write(data)
        print("SUCCESS: Binary saved successfully.\n")
    else:
        print("ERROR: No path matches found.\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python force_skip_uti.py <path_to_binary>")
    else:
        stub_missing_framework(sys.argv[1])
