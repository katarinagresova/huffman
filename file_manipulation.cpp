#include "file_manipulation.hpp"

void check_files(string ifile, string ofile, FILE **fp_in, FILE **fp_out) {
    *fp_in = fopen(ifile.c_str(), "rb");
    if (*fp_in == nullptr) {
        cerr << ifile << " could not be opened!" << endl;
        exit(1);
    }

    *fp_out = fopen(ofile.c_str(), "wb");
    if (*fp_out == nullptr) {
        cerr << ofile << " could not be opened!" << endl;
        fclose(*fp_in);
        exit(1);
    }

    //input file was empty - do empty output and finish
    fseek(*fp_in, 0, SEEK_END);
    if(ftell(*fp_in) == 0) {
        fclose(*fp_in);
        fclose(*fp_out);
        exit(0);
    }
    rewind(*fp_in);
}

/* Read the next bit to be read in the biffer and returns it.
 * fp: input file (that will be read in case the buffer limit is reached).
 * buffer: the input buffer, that can be updated in case a read operation if performed in the input file.
 * bufferSize: how many bits there are left to be read in the buffer.
 * fileSize: the filesize (until its next to last byte).
 * readHowManyBitsAtTheEnd: how many bits should be read in the next to last byte.
 * returns: the bit (0 or 1).
 */
int readBit(FILE *fp) {

    static bool first_read = true;
    static long int file_size = 0;
    static u_int8_t bits_at_the_end = 0;
    static u_int8_t buffer = 0;
    static int buffer_size = 0;
    if (first_read) {
        /* Obtains the file size (excluding the last byte). */
        fseek(fp, -1, SEEK_END);
        file_size = ftell(fp);

        /* Reads the last byte, which contains the number of bits that should be read in the previous byte */
        fread(&bits_at_the_end, sizeof(u_int8_t), 1, fp);
        rewind(fp);

        first_read = false;
    }

    if (buffer_size == 0) {
        buffer_size = 8;
        fread(&buffer, sizeof(u_int8_t), 1, fp);
    }

    if (bits_at_the_end != 8) {
        if (ftell(fp) == file_size && bits_at_the_end <= (8 - buffer_size)) return -1;
    }

    if (ftell(fp) > file_size || feof(fp)) return -1;

    buffer_size--;

    return (buffer >> buffer_size) & 1;
}

/* Reads the next 8 bits to be read and return it as a char.
 * fp: input file (that will be read in case the buffer limit is reached).
 * buffer: the input buffer, that can be updated in case a read operation if performed in the input file.
 * bufferSize: how many bits there are left to be read in the buffer.
 * fileSize: the filesize (until its next to last byte).
 * readHowManyBitsAtTheEnd: how many bits should be read in the next to last byte.
 * returns: the read byte.
 */
u_int8_t readByte(FILE *fp) {
    u_int8_t result = 0;

    int i, bit;
    for (i = 0; i < 8; i++) {
        bit = readBit(fp);
        bit = bit << (7-i);
        result |= bit;
    }

    return result;
}

/**
 * Adds bit tu 8b buffer and writes it to output file
 * @param i         bit to be written, or EOF
 * @param outfile   output file stream
 */
void write_bit(u_int8_t i, FILE *fp) {
    static int bit_pos = 0; //0 to 7 (left to right) on the byte block
    static u_int8_t c = '\0'; //byte block to write

    if (i != EOF_VAL) {
        if (i == 1) {
            c = c | (i << (7 - bit_pos)); //add a 1 to the byte
        } else {
            c = c & static_cast<unsigned char>(255 - (1 << (7 - bit_pos))); //add a 0
        }
        ++bit_pos;
        bit_pos %= 8;
        if (bit_pos == 0) {
            fwrite(&c, sizeof(u_int8_t), 1, fp);
            c = '\0';
        }
    } else {
        fwrite(&c, sizeof(u_int8_t), 1, fp);

        c = bit_pos;
        fwrite(&c, sizeof(u_int8_t), 1, fp);
    }
}

void write_byte(u_int8_t byte, FILE *fp) {
    for (int i = 0; i < 8; i++) {
        write_bit((byte >> (7 - i)) & 1, fp);
    }
}