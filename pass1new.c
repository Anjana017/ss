#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void passOne();
void displayFileContents(const char *filename);

int main() {
    passOne();  // Perform Pass One of the assembler
    return 0;
}

void passOne() {
    FILE *fp1 = fopen("input.txt", "r");
    FILE *fp2 = fopen("optab.txt", "r");
    FILE *fp3 = fopen("symtab.txt", "w");
    FILE *fp4 = fopen("intermediate.txt", "w");
    FILE *fp5 = fopen("length.txt", "w");

    char label[10], opcode[10], operand[10], code[10], mnemonic[3];
    int locctr = 0, start = 0, length = 0;

    fscanf(fp1, "%s %s %s", label, opcode, operand);

    if (strcmp(opcode, "START") == 0) {
        start = atoi(operand);
        locctr = start;
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s %s %s", label, opcode, operand);
    }

    while (strcmp(opcode, "END") != 0) {
        fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        if (strcmp(label, "") != 0) fprintf(fp3, "%s\t%d\n", label, locctr);

        fscanf(fp2, "%s %s", code, mnemonic);
        while (strcmp(code, "END") != 0) {
            if (strcmp(opcode, code) == 0) {
                locctr += 3;
                break;
            }
            fscanf(fp2, "%s %s", code, mnemonic);
        }

        if (strcmp(opcode, "WORD") == 0) locctr += 3;
        else if (strcmp(opcode, "RESW") == 0) locctr += 3 * atoi(operand);
        else if (strcmp(opcode, "BYTE") == 0) locctr++;
        else if (strcmp(opcode, "RESB") == 0) locctr += atoi(operand);

        fscanf(fp1, "%s %s %s", label, opcode, operand);
    }

    fprintf(fp4, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);

    length = locctr - start;
    fprintf(fp5, "%d", length);
    
    fclose(fp1); fclose(fp2); fclose(fp3); fclose(fp4); fclose(fp5);

    displayFileContents("input.txt");
    displayFileContents("intermediate.txt");
    displayFileContents("symtab.txt");

    printf("\nThe length of the code: %d\n", length);
}

void displayFileContents(const char *filename) {
    FILE *fp = fopen(filename, "r");
    char ch;
    printf("\nThe contents of %s:\n\n", filename);
    while ((ch = fgetc(fp)) != EOF) printf("%c", ch);
    fclose(fp);
}