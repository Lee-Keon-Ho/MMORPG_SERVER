#pragma once
#include <vector>
#include "VECTOR.h"

using Path = std::vector<VECTOR3>;

class CJob
{
private:
	VECTOR3		m_startPos;
	VECTOR3		m_endPos;
	Path*		m_path;
	bool*		m_walkable;
public:
	CJob();
	CJob(VECTOR3& _startPos, VECTOR3& _endPos, Path* _path, bool* _walkable);
	~CJob();

	Path* GetPath();
	VECTOR3& GetStartPos();
	VECTOR3& GetEndPos();
	bool* Getwalkable();
};