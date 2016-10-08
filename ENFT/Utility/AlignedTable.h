
#ifndef _ALIGNED_TABLE_H_
#define _ALIGNED_TABLE_H_

#include "Table.h"
#include "SSE.h"

template<class DATA_TYPE, typename INDEX_TYPE>
class AlignedTable : public Table<DATA_TYPE, INDEX_TYPE>
{

public:

	AlignedTable() : Table<DATA_TYPE, INDEX_TYPE>() {}
	AlignedTable(const INDEX_TYPE &nRows, const INDEX_TYPE &nCols) : Table<DATA_TYPE, INDEX_TYPE>(nRows, nCols) {}
	~AlignedTable() { Clear(); }

protected:

	virtual DATA_TYPE* Allocate(const uint &count) const
	{
		DATA_TYPE* p = (DATA_TYPE*) _aligned_malloc(count * sizeof(DATA_TYPE), SSE_ALIGNMENT);
		if(p == NULL) throw std::bad_alloc();
		return p;
	}
	virtual void Deallocate(DATA_TYPE* p) const { _aligned_free(p); }

};

#endif