#pragma once

#include <vector>

class Entity;

#define NO_PARENT -1

typedef long long entity_id;

class EntityManager {
public:
	static entity_id CreateEntity();
	static Entity* CreateAndGetEntity();
	
	static Entity* GetEntity(entity_id a_id);
	static bool IsEntityValid(entity_id a_id);
	
	static void DestroyEntity(entity_id a_id);

	class iterator {
	public:
		typedef iterator self_type;
		typedef Entity value_type;
		typedef Entity& reference;
		typedef Entity* pointer;
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
		void iterate();
		pointer m_ptr;
		pointer m_end;
	};

	static iterator begin();
	static iterator end();

private:
	static Entity* end_ptr();

	static std::vector<Entity*> s_entityTable;
	static std::vector<int> s_freeList;
};
