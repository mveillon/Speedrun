import png, array

#gen_csv: int -> void
#converts the png file with the map into a csv containing each block type
#must be run manually as c++ does not run it
def gen_csv(map_number):
    csv = open("map%d.csv" % map_number, 'w')
    base = png.Reader(filename = 'map%d.png' % map_number)
    w, h, pixels, metadata = base.read_flat()
    byte_width = 4 if metadata['alpha'] else 3
    x = 0
    try:
        for p in range(0, len(pixels), byte_width):
            block = ''
            if pixels[p] == 0:
                block = 'wall'
            elif pixels[p] == 255:
                block = 'air'
            elif pixels[p] == 100:
                block = 'spawn'
            elif pixels[p] == 50:
                block = 'end'
            elif pixels[p] == 10:
                block = 'mob'
            else:
                raise RuntimeError('Unclassified pixel (%d, %d)' % (p,
                                                                    pixels[p]))
            csv.write(block)
            if x == w - 1:
                csv.write('\n')
                x = 0
            else:
                x += 1
                csv.write(',')
    finally:
        csv.close()

gen_csv(3)
