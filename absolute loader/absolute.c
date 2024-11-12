#include <stdio.h>
#include <string.h>
void handleHeader(FILE *fp1, int *length);
void handleTextRecord(FILE *fp1, FILE *fp2, int *address, char *input);
void handleOtherRecord(FILE *fp2, int *address, char *input);
int main() {
 char input[10];
 int length = 0, address = 0;
 FILE *fp1, *fp2;
 fp1 = fopen("input.txt", "r");
 fp2 = fopen("output.txt", "w");
 if (fp1 == NULL || fp2 == NULL) {
 printf("Error opening file.\n");
 return 1;
 }
 fscanf(fp1, "%s", input);
 while (strcmp(input, "E") != 0) {
 if (strcmp(input, "H") == 0) {
 handleHeader(fp1, &length);
 } else if (strcmp(input, "T") == 0) {
 handleTextRecord(fp1, fp2, &address, input);
 } else {
 handleOtherRecord(fp2, &address, input);
 }
 fscanf(fp1, "%s", input);
 }
 fclose(fp1);
 fclose(fp2);
 printf("FINISHED\n");
 return 0;
}
void handleHeader(FILE *fp1, int *length) {
 fscanf(fp1, "%d", length);
}
void handleTextRecord(FILE *fp1, FILE *fp2, int *address, char *input) {
 fscanf(fp1, "%d", address);
 fscanf(fp1, "%s", input);
 for (int i = 0; i < 3; i++) {
 fprintf(fp2, "%d\t%c%c\n", (*address) + i, input[i * 2], input[i * 2 + 1]);
 }
 *address += 3;
}
void handleOtherRecord(FILE *fp2, int *address, char *input) {
 for (int i = 0; i < 3; i++) {
 fprintf(fp2, "%d\t%c%c\n", (*address) + i, input[i * 2], input[i * 2 + 1]);
 }
 *address += 3;
}
