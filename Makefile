all:
	(cd timerLib; make install)
	(cd lcdLib; make install)
	(cd carJumpGame; make)

doc:
	rm -rf doxygen_docs
	doxygen Doxyfile
clean:
	(cd timerLib; make clean)
	(cd lcdLib; make clean)
	(cd carJumpGame; make clean)
	rm -rf lib h
	rm -rf doxygen_docs/*
