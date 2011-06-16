mahna.xex: mahna.asx mahna.smp
	xasm -q -o $@ $< -l

mahna.smp: wav2smp.exe mahna.wav
	./wav2smp.exe mahna.wav $@

mahna.wav: mahna44k.wav
	sox -v 2.0 $< -c 1 -r 15556 -1 $@

bankier00.mic: bankowcy.exe bankowcy.bin
	./bankowcy.exe bankowcy.bin bankier%02d.mic

%.exe: %.c
	gcc -s -O2 -o $@ $<

.DELETE_ON_ERROR:
