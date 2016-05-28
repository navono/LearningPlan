#!/usr/local/bin/python

import struct
packed = struct.pack('>i4sh', 7, b'spam', 8)
file = open('I:\Sourcecode\Python\data.bin', 'wb')
file.write(packed)
file.close()

data = open('I:\Sourcecode\Python\data.bin', 'rb').read()
print(data)

#a sequence of 8-bit bytes
print(list(data))

print(struct.unpack('>i4sh', data))
