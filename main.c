/*=============================================================================
| Assignment: pa02 - Calculating an 8, 16, or 32 bit
| checksum on an ASCII input file
|
| Author: Your name here
| Language: c, c++, Java, GO, Python
|
| To Compile: javac pa02.java
| gcc -o pa02 pa02.c
| g++ -o pa02 pa02.cpp
| go build pa02.go
| python pa02.py //Caution - expecting input parameters
|
| To Execute: java -> java pa02 inputFile.txt 8
| or c++ -> ./pa02 inputFile.txt 8
| or c -> ./pa02 inputFile.txt 8
| or go -> ./pa02 inputFile.txt 8
| or python-> python pa02.py inputFile.txt 8
| where inputFile.txt is an ASCII input file
| and the number 8 could also be 16 or 32
| which are the valid checksum sizes, all
| other values are rejected with an error message
| and program termination
|
| Note: All input files are simple 8 bit ASCII input
|
| Class: CIS3360 - Security in Computing - Spring 2023
| Instructor: McAlpin
| Due Date: per assignment
|
+=============================================================================*/


#include <stdio.h>
#include <stdlib.h>

#define MAX_BUF_SIZE 80 // maximum size of buffer

void print_checksum(unsigned long checksum, int checksum_size, int character_cnt) {
    char *format_str; // format string for printing the checksum
    int hex_digits; // number of hex digits for the checksum
    
    // select format string and hex digits based on checksum size
    switch(checksum_size) {
        case 8:
            hex_digits = 2;
            format_str = "%2d bit checksum is %02lx for all %4d chars\n";
            break;
        case 16:
            hex_digits = 4;
            format_str = "%2d bit checksum is %04lx for all %4d chars\n";
            break;
        case 32:
            hex_digits = 8;
            format_str = "%2d bit checksum is %08lx for all %4d chars\n";
            break;
        default:
            fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
            exit(1);
    }
    
    // print the checksum using the selected format string and hex digits
    printf(format_str, checksum_size, checksum & ((1 << (hex_digits * 4)) - 1), character_cnt);
}

int main(int argc, char **argv) {
    // check command line arguments
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <checksum_size>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1]; // name of input file
    int checksum_size = atoi(argv[2]); // size of checksum in bits

    // validate checksum size
    if(checksum_size != 8 && checksum_size != 16 && checksum_size != 32) {
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        return 1;
    }

    // open input file
    FILE *input_file = fopen(filename, "r");
    if(input_file == NULL) {
        fprintf(stderr, "Error: could not open input file %s\n", filename);
        return 1;
    }

    // Echo the input file
    char buffer[MAX_BUF_SIZE]; // buffer for reading input file
    int character_cnt = 0; // count of characters in input file
    while(fgets(buffer, MAX_BUF_SIZE, input_file) != NULL) {
        printf("%s", buffer); // print buffer contents
        character_cnt += sizeof(buffer) - 1; // count characters in buffer, excluding null terminator
    }
    character_cnt--; // exclude the LF termination character
    if(buffer[sizeof(buffer) - 2] == '\n') {
        character_cnt++; // include the LF termination character if present
    }

    // Calculate the checksum
    unsigned long checksum = 0; // checksum value
    rewind(input_file); // reset file position to beginning of file
    while(fgets(buffer, MAX_BUF_SIZE, input_file) != NULL) { // read file contents into buffer
        for(int i = 0; i < sizeof(buffer) - 1; i++) { // iterate over buffer contents
            checksum += buffer[i]; // add character to checksum value
        }
    }

    // Pad with X if necessary
    int padding_size = checksum_size / 8 - character_cnt % (checksum_size / 8); // number of X characters to add for padding
      // if padding size equals checksum size, set padding
    if(padding_size == checksum_size / 8) { 
        padding_size = 0;
    }
    for(int i = 0; i < padding_size; i++) {
        checksum += 'X';
    }
    character_cnt += padding_size;

    // Print the checksum
    print_checksum(checksum, checksum_size, character_cnt);

    fclose(input_file);
    return 0;
}

/*=============================================================================
| I [Donald Kelly] ([4445632]) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+============================================================================*/
