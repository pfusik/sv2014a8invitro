#include <stdio.h>

int main(int argc, char *argv[])
{
	static const int col_addr[] = {
		0xe046, 0xe047, // wall
		0xe4b8, 0, 0, 0, 0, 0, 0, 0, // door closed
		0xe050, 0xe051, 0xe052, // wall
		0xe7a8, 0, 0, 0, 0, 0, // door 3/4
		0xe9dc, 0, 0, 0, // door 1/2
		0x2301, // door open
		0x4b76, 0, 0, 0, 0, 0, 0, // guy
		0x43c0, 0, 0, 0, 0, 0, 0 // babe
//		0x5ae2, 0, 0, 0, 0, 0, 0 // guy 2
	};
	static const char col_bpl[] = {
		0x36, 0x36, // wall
		8, 8, 8, 8, 8, 8, 8, 8, // door closed
		0x36, 0x36, 0x36, // wall
		6, 6, 6, 6, 6, 6, // door 3/4
		4, 4, 4, 4, // door 1/2
		0, // door open
		7, 7, 7, 7, 7, 7, 7, // guy
		7, 7, 7, 7, 7, 7, 7 // babe
//		7, 7, 7, 7, 7, 7, 7 // guy2
	};
	FILE *fp;
	unsigned char mem[65536];
	int y;
	if (argc != 3) {
		fprintf(stderr, "Usage: bank bank.mem bank.mic\n");
		return 1;
	}
	fp = fopen(argv[1], "rb");
	fread(mem, 1, sizeof(mem), fp);
	fclose(fp);

	fp = fopen(argv[2], "wb");
	for (y = 0; y < 8; y++) {
		int x;
		for (x = 0; x < 40; x++)
			fputc(mem[0xc800 + mem[0xe010 + x] * 8 + y], fp);
	}
	for (y = 0; y < 94; y++) {
		int x;
		int addr = 0;
		for (x = 0; x < sizeof(col_bpl); x++) {
			if (col_addr[x] == 0)
				addr++;
			else if (col_bpl[x] == 0x36)
				addr = 0xc800 + mem[col_addr[x] + 0x36 * (y >> 3)] * 8 + (y & 7);
			else if (y < 94)
				addr = col_addr[x] + col_bpl[x] * y;
			else
				addr = 0x100;
			fputc(mem[addr], fp);
		}
		for (; x < 40; x++)
			fputc(0, fp);
	}
	for (y = 94; y < 96 + 8; y++) {
		int x;
		for (x = 0; x < 40; x++)
			fputc(mem[0xc800 + mem[0xe046 + x + 0x36 * (y >> 3)] * 8 + (y & 7)], fp);
	}
	fputc(0x00, fp);
	fputc(0x28, fp);
	fputc(0x1a, fp);
	fputc(0x14, fp);
	fclose(fp);
	return 0;
}
