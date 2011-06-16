#include <stdio.h>

int main(int argc, char *argv[])
{
	FILE *wav;
	FILE *smp;
	int s = -1;
	if (argc != 3) {
		fprintf(stderr, "Usage: wav2smp file.wav file.smp\n");
		return 1;
	}
	wav = fopen(argv[1], "rb");
	smp = fopen(argv[2], "wb");
	if (wav == NULL || smp == NULL) {
		fprintf(stderr, "wav2smp: error opening\n");
		return 1;
	}
	fseek(wav, 44, SEEK_SET);
	for (;;) {
		int x = getc(wav);
		if (x < 0)
			break;
		x = (x + 8) >> 4;
		if (x < 0)
			x = 0;
		else if (x > 0xf)
			x = 0xf;
		if (s < 0)
			s = x << 4;
		else {
			putc(s | x, smp);
			s = -1;
		}
	}
	fclose(smp);
	fclose(wav);
	return 0;
}
