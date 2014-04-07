run: ipad.xex
	cygstart $<

ipad.xex: ipad.asx datamatrix.asx atari8.fnt
	xasm -q -l -o $@ $<

mahna.xex: mahna.asx mahna.smp bank.mic karate.mic
	xasm -q -o $@ $< -l

mahna.smp: wav2smp.exe mahnax.wav
	./wav2smp.exe mahnax.wav $@

bank.mic: bank.exe bank.mem
	./bank.exe bank.mem $@

bankier00.mic: bankowcy.exe bankowcy.bin
	./bankowcy.exe bankowcy.bin bankier%02d.mic

karate.mic: karateki.exe ik6800.bin
	./karateki.exe ik6800.bin karate%02d.mic karate.mic

%.exe: %.c
	gcc -s -O2 -o $@ $<

clean:
	$(RM) ipad.xex mahna.xex mahna.smp bank.mic bankier??.mic bank.exe karate.mic karate??.mic karate??.bin karateki.exe
.PHONY: clean

.DELETE_ON_ERROR:
