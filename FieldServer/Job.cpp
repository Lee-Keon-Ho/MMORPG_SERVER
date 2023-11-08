#include "Job.h"

CJob::CJob() : m_startPos({ 0,0,0 }), m_endPos({0,0,0}), m_path(nullptr), m_walkable(nullptr)
{
}

CJob::CJob(VECTOR3& _startPos, VECTOR3& _endPos, Path* _path, bool* _walkable) :
	m_startPos(_startPos), m_endPos(_endPos), m_path(_path), m_walkable(_walkable)
{
}

CJob::~CJob()
{
}

Path* CJob::GetPath()
{
	return m_path;
}

VECTOR3& CJob::GetStartPos()
{
	return m_startPos;
}

VECTOR3& CJob::GetEndPos()
{
	return m_endPos;
}

bool* CJob::Getwalkable()
{
	return m_walkable;
}
