#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void display();
void swap(char *x, char *y);
char* reverse(char *buffer, int i, int j);
char* itoa(int value, char* buffer, int base);

void swap(char *x, char *y) {
    char t = *x; 
    *x = *y; 
    *y = t;
}
char* reverse(char *buffer, int i, int j) {
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}
char* itoa(int value, char* buffer, int base) {
    if (base < 2 || base > 32) {
        return buffer;
    }
    
    int n = abs(value);
    int i = 0;
    while (n) {
        int r = n % base;
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10); // 'A' is 65
        } else {
            buffer[i++] = 48 + r; // '0' is 48
        }
        n = n / base;
    }
    if (i == 0) {
        buffer[i++] = '0';
    }
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
    buffer[i] = '\0';
    return reverse(buffer, 0, i - 1);
}

int main() {
    char a[10], ad[10], label[10], opcode[10], operand[10], symbol[10];
    int start, diff, address, add, len, actual_len, finaddr, prevaddr, j = 0;
    char mnemonic[15][15] = {"LDA", "STA", "LDCH", "STCH"};
    char code[15][15] = {"33", "44", "53", "57"};
    FILE *fp1 = fopen("output.txt", "w");
    FILE *fp2 = fopen("symtab.txt", "r");
    FILE *fp3 = fopen("intermediate.txt", "r");
    FILE *fp4 = fopen("objcode.txt", "w");
    if (!fp1 || !fp2 || !fp3 || !fp4) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    fscanf(fp3, "%s\t%s\t%s", label, opcode, operand);
    while (strcmp(opcode, "END") != 0) {
        prevaddr = address;
        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
    }
    finaddr = address;
    fclose(fp3);
    fp3 = fopen("intermediate.txt", "r");
    fscanf(fp3, "\t%s\t%s\t%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
        fprintf(fp4, "H^%s^00%s^00%d\n", label, operand, finaddr);
        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
        start = address;
        diff = prevaddr - start;
        fprintf(fp4, "T^00%d^%d", address, diff);
    }
    while (strcmp(opcode, "END") != 0) {
        if (strcmp(opcode, "BYTE") == 0) {
            fprintf(fp1, "%d\t%s\t%s\t%s\t", address, label, opcode, operand);
            len = strlen(operand);
            actual_len = len - 3; // Adjust length for 'C' or 'X' designator
            fprintf(fp4, "^");
            for (int i = 2; i < (actual_len + 2); i++) {   
                itoa(operand[i], ad, 16);
                fprintf(fp1, "%s", ad);
                fprintf(fp4, "%s", ad);
            }
            fprintf(fp1, "\n");
        } else if (strcmp(opcode, "WORD") == 0) {
            itoa(atoi(operand), a, 10);
            fprintf(fp1, "%d\t%s\t%s\t%s\t00000%s\n", address, label, opcode, operand, a);
            fprintf(fp4, "^00000%s", a);
        } else if (strcmp(opcode, "RESB") == 0 || strcmp(opcode, "RESW") == 0) {
            fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
        } else {
            while (j < 4 && strcmp(opcode, mnemonic[j]) != 0) j++;
            if (j < 4) { // Check if mnemonic found
                if (strcmp(operand, "COPY") == 0) {
                    fprintf(fp1, "%d\t%s\t%s\t%s\t%s0000\n", address, label, opcode, operand, code[j]);
                } else {
                    rewind(fp2);
                    fscanf(fp2, "%s%d", symbol, &add);
                    while (strcmp(operand, symbol) != 0) {
                        fscanf(fp2, "%s%d", symbol, &add);
                    }
                    fprintf(fp1, "%d\t%s\t%s\t%s\t%s%d\n", address, label, opcode, operand, code[j], add);
                    fprintf(fp4, "^%s%d", code[j], add);
                }
            }
        }
        fscanf(fp3, "%d%s%s%s", &address, label, opcode, operand);
    }
    fprintf(fp1, "%d\t%s\t%s\t%s\n", address, label, opcode, operand);
    fprintf(fp4, "\nE^00%d", start);
    fclose(fp4);
    fclose(fp3);
    fclose(fp2);
    fclose(fp1);
    return 0;
}
