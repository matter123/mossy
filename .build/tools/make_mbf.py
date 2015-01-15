import xml.etree.ElementTree as ET
from PIL import Image
import sys
import struct
import os.path as path


def make_glyph(png, cp, _x, _y):
    _bytes = struct.pack("<I", cp)
    for y in range(16):
        row = 0
        for x in range(8):
            pixel = png.getpixel((_x + x, _y + y))
            if pixel[0] == 0:
                row |= 1
            row = row << 1
        row = row >> 1
        _bytes += struct.pack("<B", row)
    for y in range(0, 16, 8):
        row = 0
        for y2 in range(8):
            pixel = png.getpixel((_x + 8, _y + y + y2))
            if pixel[0] == 0:
                row |= 1
            row = row << 1
        row = row >> 1
        _bytes += struct.pack("<B", row)
    _bytes += struct.pack("<H", 0)
    print(chr(cp) + ";" + ":".join("{:02x}".format(c) for c in _bytes))
    return (cp, chr(cp), _bytes)

tree = ET.parse(sys.argv[1])
glyphs = []
root = tree.getroot()
fol, name = path.split(path.abspath(sys.argv[1]))
name = path.splitext(name)[0]
png = Image.open(path.join(fol, root.attrib["png"]))
for g in root:
    pos = g.find("pos").attrib
    glyphs.append(make_glyph(png,
                             int(g.find("codepoint").text, 16),
                             int(pos["x"]) * 9,
                             int(pos["y"]) * 16))
glyphs.sort(key=lambda glyph: glyph[0])
gdef = -1
xdef = int(root.attrib["def"], 16)
for idx, glyph in enumerate(glyphs):
    if glyph[0] == xdef:
        gdef = idx
        break
    elif glyph[0] > xdef:
        print("error could not find def, aborting, max")
        exit()
if gdef == -1:
    print("error could not find def, aborting, nf")
    exit()
if not len(sys.argv) == 3:
    f = open(path.join(fol, name + '.mbf'), 'wb')
else:
    f = open(sys.argv[2], 'wb')
f.write(struct.pack("<III", 0x12345678, len(glyphs), 0))
f.write(struct.pack("<III", glyphs[0][0], glyphs[-1][0], gdef))
for glyph in glyphs:
    f.write(glyph[2])
f.close()
