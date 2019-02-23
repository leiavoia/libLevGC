#ifndef LIFE_OBJECT_H
#define LIFE_OBJECT_H



namespace LGC {




class LifeObject {
public:
	LifeObject( int health );

	virtual ~LifeObject() {;;}


	inline float HP() { return hp; }
	inline float MaxHP() { return maxhp; }
	inline bool Dead() { return dead; }
	inline float HPPercent() { return ( hp / maxhp ); }
	inline void Kill() { dead = true; }

	virtual void Hurt( float x );
	virtual void HurtPercent( float x ); // 0.0 - 1.0
	virtual void Heal( float x );
	virtual void HealPercent( float x ); // 0.0 - 1.0


private:
	float hp;
	float maxhp;
	bool dead;
	};




	
} // end namespace LGC	



#endif



