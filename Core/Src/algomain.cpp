#include "algomain.h"


#include <string>
#include <map>

using namespace std;

NameCounter::NameCounter() {}
void NameCounter::reset() {
	mp.clear();
}
void NameCounter::addName(string nm) {
	if (mp.count(nm))
		mp[nm]++;
	else
		mp[nm] = 1;
}
int NameCounter::highestRepeat() {
	int mx = 0;
	for (auto it : mp) {
		if (it.second > mx)
			mx = it.second;
	}
	return mx;
}


