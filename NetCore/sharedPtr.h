#pragma once
#include <iostream>
#include <assert.h>

namespace LKH
{
	template<typename T>
	class sharedPtr
	{
	private:
		T* m_ptr;
		long volatile* m_ref_count;

	public:
		sharedPtr(T* _p) : m_ptr(_p), m_ref_count(new volatile long(1)) {}

		sharedPtr(const sharedPtr<T>& _other) : m_ptr(_other.m_ptr), m_ref_count(_other.m_ref_count)
		{
			_InterlockedIncrement(m_ref_count);
		}

		~sharedPtr() { release(); }

		void release()
		{
			assert(m_ref_count);
			
			long count = _InterlockedDecrement(m_ref_count);

			int a = 0;
			if (*m_ref_count < 0)
			{
				a = 100;
			}
			if (count == 0)
			{
				delete m_ptr;
				delete m_ref_count;
			}
		}

		int useCount() { return m_ref_count ? *m_ref_count : 0; }

		T* get() { return m_ptr; }
	};
}