all:
	$(MAKE) -C mrpf
	cp mrpf/banker banker

clean:
	$(MAKE) -C mrpf clean
	rm -f banker result.txt
