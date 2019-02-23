
#include "LifeObject.h"


namespace LGC {




LifeObject::LifeObject( int health ) {
	hp = health;
	maxhp = health;
	dead = false;
	}

void LifeObject::Hurt( float x ) {
	hp -= x;
	if (hp <= 0) { dead = true; }
	}

void LifeObject::HurtPercent( float x ) {
	hp *= (1.0-x);
	if (hp <= 0) { dead = true; }
	}

void LifeObject::Heal( float x ) {
	hp += x;
	if (hp > maxhp) { hp = maxhp; }
	}

void LifeObject::HealPercent( float x ) {
	hp *= (1.0 + x);
	if (hp > maxhp) { hp = maxhp; }
	}




	
} // end namespace LGC	






