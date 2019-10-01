# huffman

Compression of grayscale image data using static and adaptive Huffman coding.

``
Usage: ./huff_codec -c/-d -h static/adaptive -i <ifile> -o <ofile> [-m]
``

Options:

        -c              Application will compress input file
        -d              Application will decompress input file
        -h static       Choose static Huffman code
        -h adaptive     Choose adaptive Huffman code
        -m              Activate model
        -i <ifile>      Path to input file
        -o <ofile>      Path to output file
        -h              Show this help message
