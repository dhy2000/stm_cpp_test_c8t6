#ifndef INC_ALGOMAIN_H_
#define INC_ALGOMAIN_H_

#include <map>
#include <string>

using std::map;
using std::string;

class NameCounter {
private:
	map<string, int> mp;
public:
	NameCounter();
	void reset() ;
	void addName(string nm) ;
	int highestRepeat() ;
};


#endif /* INC_ALGOMAIN_H_ */
