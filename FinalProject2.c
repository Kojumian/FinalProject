#include <stdio.h>

#include <stdint.h>

#include <stdlib.h>

#include <string.h>

static char chartable[] = {
  'A',
  'B',
  'C',
  'D',
  'E',
  'F',
  'G',
  'H',
  'I',
  'J',
  'K',
  'L',
  'M',
  'N',
  'O',
  'P',
  'Q',
  'R',
  'S',
  'T',
  'U',
  'V',
  'W',
  'X',
  'Y',
  'Z',
  'a',
  'b',
  'c',
  'd',
  'e',
  'f',
  'g',
  'h',
  'i',
  'j',
  'k',
  'l',
  'm',
  'n',
  'o',
  'p',
  'q',
  'r',
  's',
  't',
  'u',
  'v',
  'w',
  'x',
  'y',
  'z',
  '0',
  '1',
  '2',
  '3',
  '4',
  '5',
  '6',
  '7',
  '8',
  '9',
  '+',
  '/' // basically the entire base64 table to be applied for the algorithm
};
static char * basedtable = NULL; /* NULL being used as pointers are gonna be coming in soon. */
static int calctable[] = {
  0,
  2,
  1
};

void totaltable() {

  basedtable = malloc(256);

  for (int i = 0; i < 64; i++)
    basedtable[(unsigned char) chartable[i]] = i;
}

char * b64formula(const unsigned char * data,
  size_t ilength,
  size_t * olength) {

  * olength = 4 * ((ilength + 2) / 3);

  char * odata = malloc( * olength);
  if (odata == NULL) return NULL;

  for (int w = 0, k = 0; w < ilength;) {

    uint32_t eightgroupA = w < ilength ? (unsigned char) data[w++] : 0;
    uint32_t eightgroupB = w < ilength ? (unsigned char) data[w++] : 0;
    uint32_t eightgroupC = w < ilength ? (unsigned char) data[w++] : 0;

    uint32_t trio = (eightgroupA << 0x10) + (eightgroupB << 0x08) + eightgroupC;

    odata[k++] = chartable[(trio >> 3 * 6) & 0x3F];
    odata[k++] = chartable[(trio >> 2 * 6) & 0x3F];
    odata[k++] = chartable[(trio >> 1 * 6) & 0x3F];
    odata[k++] = chartable[(trio >> 0 * 6) & 0x3F];
  }

  for (int w = 0; w < calctable[ilength % 3]; w++)
    odata[ * olength - 1 - w] = '=';

  return odata;
}

unsigned char * solveformula(const char * data,
  size_t ilength,
  size_t * olength) {

  if (basedtable == NULL) totaltable();

  if (ilength % 4 != 0) return NULL;

  * olength = ilength / 4 * 3;
  if (data[ilength - 1] == '=')( * olength) --;  
  if (data[ilength - 2] == '=')( * olength) --;

  unsigned char * idata = malloc( * olength);
  if (idata == NULL) return NULL;

  for (int i = 0, j = 0; i < ilength;) {

    uint32_t sixgroupA = data[i] == '=' ? 0 & i++ : basedtable[data[i++]];

    uint32_t sixgroupB = data[i] == '=' ? 0 & i++ : basedtable[data[i++]];

    uint32_t sixgroupC = data[i] == '=' ? 0 & i++ : basedtable[data[i++]];

    uint32_t sixgroupD = data[i] == '=' ? 0 & i++ : basedtable[data[i++]];

    uint32_t trio = (sixgroupA << 3 * 6) +
      (sixgroupB << 2 * 6) +
      (sixgroupC << 1 * 6) +
      (sixgroupD << 0 * 6);

    if (j < * olength) idata[j++] = (trio >> 2 * 8) & 0xFF;
    if (j < * olength) idata[j++] = (trio >> 1 * 8) & 0xFF;
    if (j < * olength) idata[j++] = (trio >> 0 * 8) & 0xFF;
  }

  return idata;
}
// basically all that was the base64 algorithm in a nutshell

int main() {

  int count = 0; //
  char data[100];
  printf("Type in the Data you want encoded : \n \n");
  scanf("%s", data);

  long input_size = strlen(data);
  char * encodeddata = b64formula(data, input_size, & input_size);

  for (int i = 0; i < strlen(encodeddata); i++) {
    count++;
  } // a small for loop to count the number of characters
  printf("The Encoded data is : %s \n \n", encodeddata);
  printf("Since the Total number of characters is  %d , \n \n", count);

  if (count < 15)

  {
    printf("The data is Insufficient and is risky to be consumed in systems, try adding more characters to your data \n \n");
  } else {
    printf(" The data is Sufficient and is safe to be consumed in systems"); /* an if loop to check if the encoded data is good/secure enough to be used in different systems */
  }

  return 0;

}
