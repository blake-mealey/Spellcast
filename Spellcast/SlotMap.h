#pragma once

#include <vector>

#define INVALID_OBJECT -1
#define CHUNK_SIZE 256

typedef long long obj_id;
typedef int index;
typedef int version;

class SlotMapBase {
public:
	virtual ~SlotMapBase() = default;
};

template <typename T>
class SlotMap : public SlotMapBase {
public:
	obj_id CreateObject() {
		// If no free indices in objTable, expand the table
		if (m_freeList.empty()) {
			// Create the new chunk
			auto* chunk = new T[CHUNK_SIZE];
			const index newChunkIndex = m_objTable.size() * CHUNK_SIZE;
			for (index i = CHUNK_SIZE - 1; i >= 0; --i) {
				const index newIndex = newChunkIndex + i;
				chunk[i].m_id = newIndex;			// Initialize the ID of the new object to be its index (version 0)
				m_freeList.push_back(newIndex);		// Add the new ID to the free list because it is unused
			}
			m_objTable.push_back(chunk);
		}

		// Pop the next unused index
		const index free = m_freeList.back();
		m_freeList.pop_back();

		// Get the object at the index
		T* obj = GetObjectFromIndex(free);

		// Set the object to be active (for iteration purposes), and return its ID
		obj->m_active = true;
		return obj->m_id;
	}

	T* CreateAndGetObject() {
		return GetObject(CreateObject());
	}

	T* GetObject(const obj_id a_id) const {
		T* obj = GetObjectFromIndex(GetIndex(a_id));
		return obj->m_id != a_id ? nullptr : obj;
	}

	bool IsObjectValid(const obj_id a_id) const {
		return GetObject(a_id) != nullptr;
	}

	void DestroyObject(const obj_id a_id) {
		T* obj = GetObject(a_id);
		if (!obj) return;
		IncrementVersion(obj->m_id);
		obj->~T();
		m_freeList.push_back(a_id & 0xFFFFFFFF);
	}

	class iterator {
	public:
		typedef iterator self_type;
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;
		typedef std::forward_iterator_tag iterator_category;
		typedef int difference_type;
		explicit iterator(const SlotMap<T>& a_slotMap, index a_primaryIndex, index a_secondaryIndex) :
			m_primaryIndex(a_primaryIndex), m_secondaryIndex(a_secondaryIndex), m_slotMap(a_slotMap) {
			update_ptr();
		}

		self_type operator++(int) {			// postfix
			const self_type i = *this;
			iterate();
			return i;
		}
		self_type operator++() {			// prefix
			iterate();
			return *this;
		}
		reference operator*() const { return *m_ptr; }
		pointer operator->() const {return m_ptr; }
		bool operator==(const self_type& a_rhs) const { return m_ptr == a_rhs.m_ptr; }
		bool operator!=(const self_type& a_rhs) const { return m_ptr != a_rhs.m_ptr; }
	private:
		void update_ptr() {
			m_ptr = m_slotMap.m_objTable[m_primaryIndex] + m_secondaryIndex;
		}

		void iterate() {
			do {
				if (++m_secondaryIndex == CHUNK_SIZE) {
					m_secondaryIndex = 0;
					++m_primaryIndex;
				}
				// if (++m_secondaryIndex % CHUNK_SIZE == 0) ++m_primaryIndex;		// Why doesn't this work?
				update_ptr();
			} while (m_primaryIndex < m_slotMap.m_objTable.size() && !m_ptr->m_active);
		}

		pointer m_ptr;

		index m_primaryIndex;
		index m_secondaryIndex;
		const SlotMap<T>& m_slotMap;
	};

	iterator begin() { return iterator(*this, 0, 0); }
	iterator end() { return iterator(*this, m_objTable.size(), 0); }

private:
	friend iterator;
	static index GetIndex(const obj_id a_id) {
		return a_id & 0xFFFFFFFF;
	}

	static version GetVersion(const obj_id a_id) {
		return a_id >> 32;
	}

	static obj_id GetId(const index a_index, const version a_version) {
		return a_index | (a_version << 32);
	}

	static void IncrementVersion(obj_id& a_id) {
		a_id = GetId(GetIndex(a_id), GetVersion(a_id) + 1);
	}

	T* GetObjectFromIndex(const index a_index) const {
		return m_objTable[a_index / CHUNK_SIZE] + (a_index % CHUNK_SIZE);
	}

	std::vector<T*> m_objTable;
	std::vector<index> m_freeList;
};
