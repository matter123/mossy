#!/usr/bin/env python3
no_img = False
try:
    import message
except:
    class message:
        pass
    message.warn = lambda x: print(x)
    message.info = lambda x: print(x)
    message.error = lambda x: print(x)
import xml.etree.ElementTree as ET
try:
    from PIL import Image
except ImportError:
    try:
        import Image
    except ImportError:
        message.warn('PIL or Pillow not installed')
        no_img = True

import sys
import struct
import os.path as path

show_font = False
for arg in sys.argv:
    if arg.startswith('--show-font'):
        show_font = True


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
    if show_font:
        message.info(chr(cp) + ";" +
                     ":".join("{:02x}".format(c) for c in _bytes))
    return (cp, chr(cp), _bytes)


def make_mbf(infile, outfile):
    if no_img:
        return True
    try:
        tree = ET.parse(infile)
        glyphs = []
        root = tree.getroot()
        fol, name = path.split(path.abspath(infile))
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
                message.error("could not find def, aborting, max")
                exit()
        if gdef == -1:
            message.error("error could not find def, aborting, nf")
            exit()
        with open(outfile, 'wb') as f:
            f.write(struct.pack("<III", 0x12345678, len(glyphs), 0))
            f.write(struct.pack("<III", glyphs[0][0], glyphs[-1][0], gdef))
            for glyph in glyphs:
                f.write(glyph[2])
            f.close()
        return True
    except:
        return False


if not len(sys.argv) == 3:
    fol, name = path.split(path.abspath(sys.argv[1]))
    ofile = path.join(fol, name + '.mbf')
else:
    ofile = sys.argv[2]
make_mbf(sys.argv[1], ofile)
