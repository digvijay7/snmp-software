#include "Parse.h"

int main(){
	Parse p("snmptraps.log");
	p.Execute();
	int i=0;
	return 0;
}
