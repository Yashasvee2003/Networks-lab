// only contains logic req for hamming code
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void redundant_bits(int m, int r, char data[], char msg[]);
void encode(int m, int r, char data[], char msg[]);
void decode(int m, int r, char data[], char msg[]);

int main(int argc, char *argv[]) {
  char data[20];
  int m;
  printf("Enter data : ");
  scanf("%s", data);
  m = strlen(data);
  int r = 0;
  while (pow(2, r) < m + r + 1) {
    r++;
  }
  printf("Redundant bit values : %d\n", r);
  char msg[m + r + 1];
  redundant_bits(m, r, data, msg);
  printf("%s\n", msg);

  encode(m, r, data, msg);
  printf("%s\n", msg);

  msg[6] = '0';
  decode(m, r, data, msg);
}

void redundant_bits(int m, int r, char data[], char msg[]) {
  int power = 0;
  int data_idx = m - 1;
  for (int index = 0; index < m + r; index++) {
    if (index == (pow(2, power) - 1)) {
      msg[index] = 'r';
      power++;
    } else {
      msg[index] = data[data_idx--];
    }
  }
  msg[m + r] = '\0';
}

void encode(int m, int r, char data[], char msg[]) {
  for (int i = 0; i < r; i++) {
    int ones_count = 0;
    int start = pow(2, i) - 1;
    int jump = pow(2, i);

    while (start < m + r) {
      for (int j = start; j < start + jump && j < m + r; j++) {
        printf("J value : %d, bit : %c\n", j, msg[j]);
        ones_count += (msg[j] == '1');
      }

      start += pow(2, i + 1);
    }

    int pos = pow(2, i) - 1;
    msg[pos] = (ones_count % 2 == 0) ? '0' : '1';

    // printf("R value : %c\n", msg[pos]);
    // printf("ones count : %d\n", ones_count);
  }
}

void decode(int m, int r, char data[], char msg[]) {
  int binary[4];
  for (int i = 0; i < r; i++) {
    int ones_count = 0;
    int start = pow(2, i) - 1;
    int jump = pow(2, i);

    while (start < m + r) {
      for (int j = start; j < start + jump && j < m + r; j++) {
        // printf("J value : %d, bit : %c\n", j, msg[j]);
        ones_count += (msg[j] == '1');
      }

      start += pow(2, i + 1);
    }
    binary[i] = (ones_count % 2 == 0) ? 0 : 1;
    //printf("R value : %c\n", msg[pos]);
    printf("ones count : %d\n", ones_count);
  }

  int num = 0;
  for (int i = 0; i < r; i++) {
    num += pow(2,i) * binary[i];
  }
  printf("num : %d\n",num);
}
