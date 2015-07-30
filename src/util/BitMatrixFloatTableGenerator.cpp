#include <cstdio>

int main() {
  fprintf(stdout, "// Auto-generated by %s\n", __FILE__);
  fprintf(stdout, "namespace bit_matrix {\n", __FILE__);
  fprintf(stdout, "float floatTable[256][8] = {\n");
  for (int i=0; i<256; ++i) {
    int bits[8];
    for (int j=0; j<8; ++j) {
      bits[j] = (i & (1<<j)) ? 1 : 0;
    }

    fprintf(stdout, "\t%d, %d, %d, %d, %d, %d, %d, %d%c  // %d\n",
        bits[7], bits[6], bits[5], bits[4], bits[3], bits[2], bits[1], bits[0],
        (i==255) ? ' ' : ',', i);
  }

  fprintf(stdout, "};\n");
  fprintf(stdout, "}\n");
  return 0;
}