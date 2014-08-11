#include "Creep.h"

USING_NS_CC;

bool Creep::init(){
	if (!Node::init()){
		return false;
	}
	return true;
}