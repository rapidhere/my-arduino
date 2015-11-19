all: run

PI=platformio

.PHONY: run upload clean cleanall

run:
	@$(PI) run -vvv

upload:
	@$(PI) run -t upload -vvv

clean:
	@$(PI) run -t clean

cleanall: clean
	@rm -rf .pioenvs/ .sconsign.dblite
