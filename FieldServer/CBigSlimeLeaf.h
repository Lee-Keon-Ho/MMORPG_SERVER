#pragma once
#include "Monster.h"

class CBigSlimeLeaf : public CMonster
{
private:

public:
	CBigSlimeLeaf();
	CBigSlimeLeaf(VECTOR3 _position, VECTOR2_INT _rangeMin, VECTOR2_INT _rangeMax, int _type, int _index);
	~CBigSlimeLeaf();

	void Update(float _deltaTick);

private:
	void Idle(float _deltaTick) override; 
	void TargetRun(float _deltaTick);
	void CheckMonsterInRange();
};