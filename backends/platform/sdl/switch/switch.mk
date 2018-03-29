scummvm.nro: $(EXECUTABLE)
	elf2nro $(EXECUTABLE) scummvm.nro
	cp $(srcdir)/backends/vkeybd/packs/vkeybd_default.zip /home/cpasjuste/.config/RyuFs/sdmc/switch/scummvm/data
	cp $(srcdir)/backends/vkeybd/packs/vkeybd_small.zip /home/cpasjuste/.config/RyuFs/sdmc/switch/scummvm/data
	cp $(DIST_FILES_THEMES) /home/cpasjuste/.config/RyuFs/sdmc/switch/scummvm/data
ifdef DIST_FILES_ENGINEDATA
	cp $(DIST_FILES_ENGINEDATA) /home/cpasjuste/.config/RyuFs/sdmc/switch/scummvm/data
endif

.PHONY: scummvm.nro

