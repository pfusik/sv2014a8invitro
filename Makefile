run: sv2014in.xex
	cygstart $<

sv2014in.xex: loader.asx mahna.smp.0.dfl mahna.smp.1.dfl mahna.smp.2.dfl mahna.smp.3.dfl mahna.smp.4.dfl bank.dfl ipad.dfl karate.dfl
	xasm -q -l -o $@ $<

mahna.smp.0.dfl: mahna.smp
	dd if=$< bs=16K count=1 skip=0 | 7z a -tgzip -mx=9 -si -so dummy | gzip2deflate >$@

mahna.smp.1.dfl: mahna.smp
	dd if=$< bs=16K count=1 skip=1 | 7z a -tgzip -mx=9 -si -so dummy | gzip2deflate >$@

mahna.smp.2.dfl: mahna.smp
	dd if=$< bs=16K count=1 skip=2 | 7z a -tgzip -mx=9 -si -so dummy | gzip2deflate >$@

mahna.smp.3.dfl: mahna.smp
	dd if=$< bs=16K count=1 skip=3 | 7z a -tgzip -mx=9 -si -so dummy | gzip2deflate >$@

mahna.smp.4.dfl: mahna.smp
	dd if=$< bs=1K count=22 skip=64 | 7z a -tgzip -mx=9 -si -so dummy | gzip2deflate >$@

bank.bin: bank.asx dasomen.dfl bank.mic
	xasm -q -l -o $@ $<

ipad.bin: ipad.asx dasbabka.mic datamatrix.asx mahna.asx bank.asx karate.asx dasomen.asx dasgaly.mic dasgeba.mic dasoczka.mic dascmok.mic karate.mic dasomen.mic bank.mic karate.mic
	xasm -q -l -o $@ $<

karate.bin: karate.asx karate.mic
	xasm -q -l -o $@ $<

dasomen.bin: dasomen.asx dasgaly.mic dasgeba.mic dasoczka.mic dascmok.mic dasomen.mic
	xasm -q -l -o $@ $<

mahna.smp: wav2smp.exe mahnax.wav
	./wav2smp.exe mahnax.wav $@

bank.mic: bank.exe bank.mem
	./bank.exe bank.mem $@

bankier00.mic: bankowcy.exe bankowcy.bin
	./bankowcy.exe bankowcy.bin bankier%02d.mic

karate.mic: karateki.exe ik6800.bin
	./karateki.exe ik6800.bin karate%02d.mic karate.mic

%.dfl: %.bin
	7z a -tgzip -mx=9 -so dummy $< | gzip2deflate >$@

%.exe: %.c
	gcc -s -O2 -o $@ $<

clean:
	$(RM) sv2014in.xex mahna.smp.0.dfl mahna.smp.1.dfl mahna.smp.2.dfl mahna.smp.3.dfl mahna.smp.4.dfl bank.dfl ipad.dfl karate.dfl dasomen.dfl \
		bank.bin ipad.bin karate.bin dasomen.bin loader.lst bank.lst ipad.lst karate.lst dasomen.lst \
		mahna.smp wav2smp.exe bank.mic bank.exe bankier??.mic bankowcy.exe karate.mic karate??.mic karate??.bin karateki.exe

.PHONY: run clean

.DELETE_ON_ERROR:
