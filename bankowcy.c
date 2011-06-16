#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char buf[30000];
	int n;
	int i;
	if (argc != 3) {
		fprintf(stderr, "Usage: bankowcy bankowcy.bin bankier%02d.mic\n");
		return 1;
	}
	fp = fopen(argv[1], "rb");
	n = fread(buf, 1, sizeof(buf), fp) / (7 * 94);
	fclose(fp);
	for (i = 0; i < n; i++) {
		char filename[FILENAME_MAX];
		int y;
		sprintf(filename, argv[2], i);
		fp = fopen(filename, "wb");
		for (y = 0; y < 94; y++) {
			static char empty[33];
			fwrite(buf + 7 * (94 * i + y), 1, 7, fp);
			fwrite(empty, 1, 33, fp);
		}
		fputc(0x00, fp);
		fputc(0x28, fp);
		fputc(0x1a, fp);
		fputc(0x14, fp);
		fclose(fp);
	}
	return 0;
}
