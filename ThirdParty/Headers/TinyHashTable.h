#ifndef TinyHashTable_h__
#define TinyHashTable_h__

#define NULL					0
#define DEFAULT_HASH_SIZE		256

template<typename Key, typename Value>
class TinyHashTable
{
public:
	explicit TinyHashTable() :m_iHashSize(DEFAULT_HASH_SIZE) { m_pHashTable = new Hash_Node[m_iHashSize]; }
	explicit TinyHashTable(int _iSize) :m_iHashSize(_iSize) { m_pHashTable = new Hash_Node[m_iHashSize]; }
	virtual ~TinyHashTable() { this->Clear(true); }

public:
	Value&			Add(const Key& _key, Value _val);
	Value			Find(const Key& _key);
	void			Delete(const Key& _key);
	void			Clear(bool _bMemClear = false);

	Value& operator[](const Key& _key);

private:
	struct Data_Node {
		const Key	pPrimitiveKey;
		Value		pData;
		Data_Node*	pNextNode;
		Data_Node() :pPrimitiveKey(NULL), pData(NULL), pNextNode(NULL) {}
		Data_Node(const Key& _key, const Value& _Value) :pPrimitiveKey(_key), pData(_Value), pNextNode(NULL) {}
	};

	struct Hash_Node {
		Data_Node*	pDataNode;
		int			DataCount;
		Hash_Node() :pDataNode(NULL), DataCount(0) {}
	};

	Hash_Node*		m_pHashTable;
	int				m_iHashSize;

private:
	int Hash_Function(const Key& _key) { return (int)_key % m_iHashSize; }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Key, typename Value>
Value& TinyHashTable<Key, Value>::Add(const Key& _key, Value _val)
{
	int HashIndex = this->Hash_Function(_key);
	if (m_pHashTable[HashIndex].DataCount)
	{
		Data_Node* pCurrentNode = m_pHashTable[HashIndex].pDataNode;

		while (pCurrentNode->pNextNode)
		{
			if (pCurrentNode->pPrimitiveKey == _key)
				return pCurrentNode->pData;
			pCurrentNode = pCurrentNode->pNextNode;
		}

		pCurrentNode->pNextNode = new Data_Node(_key, _val);
		m_pHashTable[HashIndex].DataCount++;
		return pCurrentNode->pData;
	}
	else
	{
		m_pHashTable[HashIndex].pDataNode = new Data_Node(_key, _val);
		m_pHashTable[HashIndex].DataCount++;
		return m_pHashTable[HashIndex].pDataNode->pData;
	}
}

template<typename Key, typename Value>
Value TinyHashTable<Key, Value>::Find(const Key& _key)
{
	int HashIndex = this->Hash_Function(_key);
	if (m_pHashTable[HashIndex].DataCount)
	{
		Data_Node* pCurrentNode = m_pHashTable[HashIndex].pDataNode;

		while (pCurrentNode)
		{
			if (pCurrentNode->pPrimitiveKey == _key)
				return pCurrentNode->pData;
			pCurrentNode = pCurrentNode->pNextNode;
		}
	}

	return NULL;
}

template<typename Key, typename Value>
void TinyHashTable<Key, Value>::Delete(const Key& _key)
{
	int HashIndex = this->Hash_Function(_key);
	if (!m_pHashTable[HashIndex].DataCount) return;

	Data_Node* pPrevNode = m_pHashTable[HashIndex].pDataNode;
	Data_Node* pCurrentNode = pPrevNode->pNextNode;

	if (m_pHashTable[HashIndex].pDataNode->pPrimitiveKey == _key)
	{
		delete m_pHashTable[HashIndex].pDataNode;
		m_pHashTable[HashIndex].pDataNode = pCurrentNode;
		m_pHashTable[HashIndex].DataCount--;
		return;
	}

	while (pCurrentNode)
	{
		if (pCurrentNode->pPrimitiveKey == _key)
		{
			pPrevNode->pNextNode = pCurrentNode->pNextNode;
			delete pCurrentNode;
			m_pHashTable[HashIndex].DataCount--;
			return;
		}
		pPrevNode = pCurrentNode;
		pCurrentNode = pCurrentNode->pNextNode;
	}
}

template<typename Key, typename Value>
void TinyHashTable<Key, Value>::Clear(bool _bMemClear /*= false*/)
{
	for (int i = 0; i < m_iHashSize; ++i)
	{
		if (!m_pHashTable[i].pDataNode) continue;

		Data_Node* pDeleteNode = m_pHashTable[i].pDataNode;
		Data_Node* pCurrentNode = m_pHashTable[i].pDataNode->pNextNode;

		while (true)
		{
			delete pDeleteNode;
			pDeleteNode = pCurrentNode;
			if (!pDeleteNode) break;
			pCurrentNode = pCurrentNode->pNextNode;
		}

		m_pHashTable[i].pDataNode = NULL;
		m_pHashTable[i].DataCount = 0;
	}

	if(_bMemClear && m_pHashTable)
	{
		delete[] m_pHashTable;
		m_pHashTable = NULL;
		m_iHashSize = 0;
	}
}

template<typename Key, typename Value>
Value& TinyHashTable<Key, Value>::operator[](const Key& _key)
{
	int HashIndex = this->Hash_Function(_key);

	if (m_pHashTable[HashIndex].DataCount)
	{
		Data_Node* pPrevNode = m_pHashTable[HashIndex].pDataNode;

		if (pPrevNode) {
			if (pPrevNode->pPrimitiveKey == _key)
				return pPrevNode->pData;

			Data_Node* pCurrentNode = pPrevNode->pNextNode;
			while (pCurrentNode)
			{
				if (pCurrentNode->pPrimitiveKey == _key)
					return pCurrentNode->pData;

				pPrevNode = pCurrentNode;
				pCurrentNode = pCurrentNode->pNextNode;
			}
		}

		pPrevNode->pNextNode = new Data_Node(_key, NULL);
		m_pHashTable[HashIndex].DataCount++;
		return pPrevNode->pNextNode->pData;
	}
	else
	{
		m_pHashTable[HashIndex].pDataNode = new Data_Node(_key, NULL);
		m_pHashTable[HashIndex].DataCount++;
		return m_pHashTable[HashIndex].pDataNode->pData;
	}
}

#endif // TinyHashTable_h__
