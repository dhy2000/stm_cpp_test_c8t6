#include "bridge.h"
#include "algomain.h"

NameCounter counter;

void nm_rst() {
	counter.reset();
}

void nm_addname(const char *nm_cstr) {
	string nm(nm_cstr);
	counter.addName(nm);
}

int nm_highestrepeat() {
	return counter.highestRepeat();
}
