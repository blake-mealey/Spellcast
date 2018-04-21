#pragma once

#include <vector>

#define INVALID_OBJECT -1
#define CHUNK_SIZE 3

typedef long long obj_id;

class SlotMapBase {
public:
	virtual ~SlotMapBase() = default;
};

template <typename T>
class SlotMap : public SlotMapBase {
public:
	obj_id CreateObject() {
		if (m_freeList.empty()) {
			auto* chunk = new T[CHUNK_SIZE];
			for (int i = CHUNK_SIZE - 1; i >= 0; --i) {
				chunk[i].m_id = m_objTable.size() * CHUNK_SIZE + i;
				m_freeList.push_back(m_objTable.size() * CHUNK_SIZE + i);
			}
			m_objTable.push_back(chunk);
		}

		const int free = m_freeList.back();
		m_freeList.pop_back();
		T& obj = m_objTable[free / CHUNK_SIZE][free % CHUNK_SIZE];
		obj.m_active = true;
		return obj.m_id;
	}

	T* CreateAndGetObject() {
		return GetObject(CreateObject());
	}

	T* GetObject(const obj_id a_id) const {
		const int lowOrder = a_id & 0xFFFFFFFF;
		T* obj = m_objTable[lowOrder / CHUNK_SIZE] + (lowOrder % CHUNK_SIZE);
		return obj->m_id != a_id ? nullptr : obj;
	}

	bool IsObjectValid(const obj_id a_id) const {
		return GetObject(a_id) != nullptr;
	}

	void DestroyObject(const obj_id a_id) {
		T* obj = GetObject(a_id);
		if (!obj) return;
		obj->m_id = (a_id & 0xFFFFFFFF) | (((a_id >> 32) + 1) << 32);
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
		explicit iterator(pointer a_ptr, pointer a_end) : m_ptr(a_ptr), m_end(a_end) {}
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
		void iterate() {
			do {
				++m_ptr;
			} while (!m_ptr->m_active && m_ptr != m_end);
		}
		pointer m_ptr;
		pointer m_end;
	};

	iterator begin() { return iterator(m_objTable[0], end_ptr()); }
	iterator end() { return iterator(end_ptr(), end_ptr()); }

private:
	T* end_ptr() {
		return m_objTable[0] + m_objTable.size()*CHUNK_SIZE;
	}

	std::vector<T*> m_objTable;
	std::vector<int> m_freeList;
};
