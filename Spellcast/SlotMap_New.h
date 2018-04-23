#pragma once

#include <vector>

#define INVALID_OBJECT -1
#define CHUNK_SIZE 3

#define GET_INDEX(id) ((id & 0xFFFFFFFF))
#define GET_VERSION(id) ((id >> 32))
#define GET_ID(index, version) (index | (version << 32))

typedef long long id;
typedef id obj_id;
typedef id table_id;
typedef unsigned int index;
typedef index id_tab_index;
typedef index obj_vec_index;
typedef unsigned int version;

class SlotMapBase {
public:
	virtual ~SlotMapBase() = default;
};

template <class T>
class SlotMap : public SlotMapBase {
public:
	obj_id CreateObject() {
		// TODO: Expand freeList and idTable if free is empty

		// Get the next free index
		const index free = m_freeList.back();
		m_freeList.pop_back();

		// Update the id table to point to the back of the object vector
		table_id& id = m_idTable[free];
		SetIndex(id, m_objList.size());

		// Create a new object with an id pointing to free with the same version
		T obj;
		obj.m_id = GET_ID(free, GET_VERSION(id));
		m_objList.push_back(obj);

		// TODO: Erase table?
	}

	T* CreateAndGetObject() {
		return GetObject(CreateObject());
	}

	T* GetObject(obj_id a_id) {
		const table_id id = m_idTable[GET_INDEX(a_id)];
		if (GET_VERSION(id) != GET_VERSION(a_id)) return nullptr;
		T* obj = &(m_objList[GET_VERSION(id)]);
		return obj;
	}

	bool IsObjectValid(obj_id a_id) {
		return GetObject(a_id) != nullptr;
	}

	bool DestroyObject(obj_id a_id) {
		T* obj = GetObject(a_id);
		if (!obj) return false;
		table_id& id = m_idTable[GET_INDEX(a_id)];
		IncrementVersion(id);
		m_freeList.push_back(GET_INDEX(a_id));
		
		obj->~T();																			// Deconstruct object
		m_objList[GET_INDEX(id)] = m_objList.back();										// Replace with back of obj list
		m_objList.pop_back();																// Pop back of obj list (invalidates pointers)
		SetIndex(m_idTable[GET_INDEX(m_objList[GET_INDEX(id)].m_id)], GET_INDEX(a_id));		// 

		return true;
	}

	typename std::vector<T*>::iterator& begin() {
		return m_objList.begin();
	}

	typename std::vector<T*>::iterator& end() {
		return m_objList.end();
	}

private:
	static void IncrementVersion(id& a_id) {
		a_id = GET_ID(GET_INDEX(a_id), GET_VERSION(a_id) + 1);
	}

	static void SetIndex(id& a_id, const index a_index) {
		a_id = GET_ID(a_index, GET_VERSION(a_id));
	}

	std::vector<table_id> m_idTable;			// Table of indices/versions into m_objList
	//std::vector<index> m_eraseList;			// List of indices into m_indexTable (corresponding to m_objList)
	std::vector<T> m_objList;				// List of T objects
	std::vector<index> m_freeList;			// Queue of indices in m_indexTable
};