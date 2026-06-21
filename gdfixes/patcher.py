import os
username = os.getlogin()
path = f'/Users/{username}/Library/Application Support/Steam/steamapps/common/Geometry Dash/Geometry Dash.app/Contents/MacOS/Geometry Dash'
with open(path, 'rb') as f: data = f.read()
old_bytes = b'\x32\x00\x00\x00\x20\x00\x00\x00\x01\x00\x00\x00\x00\x00\x0B\x00'
new_bytes = b'\x32\x00\x00\x00\x20\x00\x00\x00\x01\x00\x00\x00\x00\x07\x0A\x00'
if old_bytes in data:
    with open(path, 'wb') as f: f.write(data.replace(old_bytes, new_bytes))
    print('Patch successful')
else:
    print('Patch failed')