all:: grpn

install grpn::
	$(MAKE) -C src $<

clean::
	rm -f *~
	$(MAKE) -C src clean
