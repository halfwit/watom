include ../config.mk

MAN =           \
	watom.1

.POSIX:

install: $(MAN)
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1/
	cp -f $(MAN) $(DESTDIR)$(MANPREFIX)/man1/

uninstall:
	@echo "uninstalling manpages"
	@for page in $(MAN); do \
		rm -f $(DESTDIR)$(MANPREFIX)/man1/$$page; \
	done
