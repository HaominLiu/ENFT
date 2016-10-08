
#ifndef _POOL_H_
#define _POOL_H_

template<class TYPE>
class Pool
{

public:

	Pool() {}
	~Pool()
	{
		const int N = int(m_data.size());
		for(int i = 0; i < N; ++i)
			delete m_data[i];
	}

	inline void Initialize()
	{
		m_cnt = 0;
	}

	inline TYPE* Create()
	{
		if(m_data.size() < m_cnt + 1)
			m_data.push_back(new TYPE);
		return m_data[m_cnt++];
	}

	inline void Clear()
	{
		const int N = int(m_data.size());
		for(int i = 0; i < N; ++i)
			delete m_data[i];
		m_data.resize(0);
	}

protected:

	std::vector<TYPE *> m_data;
	int m_cnt;

};

#endif