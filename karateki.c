#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char buf[30000];
	unsigned char mic[40 * 64];
	int i;
	if (argc != 4) {
		fprintf(stderr, "Usage: karateki ik6800.bin karate%02d.mic karate.mic\n");
		return 1;
	}
	fp = fopen(argv[1], "rb");
	fread(buf, 1, sizeof(buf), fp);
	fclose(fp);
	memset(mic, 0xff, sizeof(mic));
	for (i = 0; i < 56; i++) {
		int shape = buf[i * 2] + 256 * buf[i * 2 + 1] - 0x6800;
		int gfx = buf[128 + i * 2] + 256 * buf[129 + i * 2] - 0x6800;
		char filename[FILENAME_MAX];
		unsigned char out[7 * 8 * 8];
		int bad = 0;
		int maxx = 0;
		int y;
		sprintf(filename, argv[2], i);
		fp = fopen(filename, "wb");
		memset(out, 0xff, sizeof(out));
		for (y = 0; y < 12 * 8; y++) {
			int c = buf[shape + (y >> 3)];
			int p = c >> 4;
			int x;
			c &= 0xf;
			if (maxx < p + c)
				maxx = p + c;
			for (x = 0; x < p; x++)
				fputc(0, fp);
			for (x = 0; x < c; x++) {
				unsigned char b = buf[gfx + 8 * x + (y & 7)];
				fputc(b, fp);
				if (y < 32 || p + x >= 7)
					bad = 1;
				else {
					int o;
					out[y - 32 + 64 * (p + x)] = ~b;
					switch (i) {
					case 0: o = 0; break;
					case 23: o = 7; break;
					case 30: o = 13; break;
					case 48: o = 18; break;
					case 49: o = 21; break;
					case 50: o = 25; break;
					case 51: o = 32; break;
					default: o = -1; break;
					}
					if (o >= 0)
						mic[40 * (y - 32) + (o + p + x)] = ~b;
				}
			}
			for (x = p + c; x < 40; x++)
				fputc(0, fp);
			if ((y & 7) == 7)
				gfx += 8 * c;
		}
		fputc(0x06, fp);
		fputc(0x00, fp);
		fputc(0x3a, fp);
		fputc(0x0e, fp);
		fclose(fp);
		switch (i) {
		case 0:
		case 23:
		case 30:
		case 48:
		case 49:
		case 50:
		case 51:
			if (bad) {
				fprintf(stderr, "frame %d doesn't fit\n", i);
				return 1;
			}
			printf("frame %d width %d\n", i, maxx);
			sprintf(filename, "karate%02d.bin", i);
			fp = fopen(filename, "wb");
			fwrite(out, 1, sizeof(out), fp);
			fclose(fp);
			break;
		default:
			break;
		}
	}

	fp = fopen(argv[3], "wb");
	fwrite(mic, 1, sizeof(mic), fp);
	fputc(0x0e, fp);
	fputc(0x3a, fp);
	fputc(0x00, fp);
	fputc(0x06, fp);
	fclose(fp);

	fp = fopen("ik558d.bin", "rb");
	fread(buf, 1, sizeof(buf), fp);
	fclose(fp);
	for (i = 0; i < 35; i++) {
		int j;
		printf("%d: ", i);
		for (j = buf[i]; j < buf[i + 1]; j++)
			printf(" %d", buf[36 + j]);
		printf("\n");
	}
	return 0;
}
