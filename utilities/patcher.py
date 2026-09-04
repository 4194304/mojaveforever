import os
import sys

if len(sys.argv) < 2:
	print("Please provide an absolute path")
	sys.exit(1)
	
path = sys.argv[1]

with open(path, 'rb') as f: data = f.read()
old_bytes = b'\x32\x00\x00\x00\x20\x00\x00\x00\x01\x00\x00\x00\x00\x0F\x0A\x00'
new_bytes = b'\x32\x00\x00\x00\x20\x00\x00\x00\x01\x00\x00\x00\x00\x07\x0A\x00'
if old_bytes in data:
    with open(path, 'wb') as f: f.write(data.replace(old_bytes, new_bytes))
    print('Patch successful')
else:
    print('Patch failed')