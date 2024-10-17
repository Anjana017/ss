#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char input[10], bitmask[12], relocbit;
    int start, address, opcode, addr, actualadd, i;
    FILE *fp1, *fp2;

    // Input the starting address
    printf("Enter the actual starting address: ");
    scanf("%d", &start);

    // Open input/output files
    if ((fp1 = fopen("relinput.txt", "r")) == NULL || (fp2 = fopen("reloutput.txt", "w")) == NULL) {
        printf("Error: Unable to open files.\n");
        return 1;
    }

    // Process input file
    while (fscanf(fp1, "%s", input) != EOF && strcmp(input, "E") != 0) {
        if (strcmp(input, "H") == 0) {
            fscanf(fp1, "%*s %*s"); // Skip header start address and length
        } else if (strcmp(input, "T") == 0) {
            fscanf(fp1, "%x %s", &address, bitmask);
            address += start;
            
            // Process instructions
            for (i = 0; i < strlen(bitmask); i++) {
                if (fscanf(fp1, "%d %x", &opcode, &addr) != 2) {
                    printf("Error reading opcode/address at index %d.\n", i);
                    break;
                }
                relocbit = bitmask[i];
                actualadd = (relocbit == '1') ? addr + start : addr;
                fprintf(fp2, "%d\t%d\t%d\n", address, opcode, actualadd);
                address += 3; // Next instruction address
            }
        }
    }

    // Close files
    fclose(fp1);
    fclose(fp2);
    printf("FINISHED. Check 'reloutput.txt'.\n");

    return 0;
}