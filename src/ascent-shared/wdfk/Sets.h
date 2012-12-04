#ifndef WDFK_SET_H_
#define WDFK_SET_H_

#include "Base.h"
#include "Defines.h"

namespace WDFK {
	template < typename T, typename _Comp = comparable<T> >
	class set;
	template < typename T, typename _Comp = comparable<T> >
	class vector;

	template < typename T, typename _Comp = comparable<T> >
	class _set_iterator
	{
		friend class set<T,_Comp>;
		friend class vector<T,_Comp>;
	public:
		/****************************************
		* constructeurs / destructeurs
		****************************************/
		_set_iterator() { _current = NULL; }
		_set_iterator(const _set_iterator& arg) : _current(arg._current) { }
		~_set_iterator() { }

		/****************************************
		* modifications
		****************************************/
		_set_iterator& operator++() { ++_current; return *this; }
		_set_iterator operator++(int) { _set_iterator t = *this; ++*this; return t; }
		_set_iterator& operator--() { --_current; return *this; }
		_set_iterator operator--(int) { _set_iterator t = *this; --*this; return t; }
		_set_iterator& operator=(const _set_iterator& arg) { _current = arg._current; return *this; }

		/****************************************
		* tests
		****************************************/
		bool operator==(const _set_iterator& arg) const { return _current == arg._current; }
		bool operator!=(const _set_iterator& arg) const { return _current != arg._current; }
		
		/****************************************
		* extraction de la valeur
		****************************************/
		operator T*() { return _current; }
		_set_iterator& operator >>(T& arg) { arg = *_current++; }

	protected:
		T *_current;
	};

	template < typename T, typename _Comp > class set : public ArrayAllocator<T>
	{
	public:
		typedef _set_iterator<T,_Comp> const_iterator;

		/****************************************
		* Constructeurs / Destructeurs
		****************************************/
		set() { _init(&m_Set); }
		set(set& arg) { _init(&m_Set); assign(arg); }
		set(uint32 grow) : ArrayAllocator<T>(grow) { _init(&m_Set); }

		/****************************************
		* manipulations
		****************************************/
		set<T,_Comp>& insert(const T& key)
		{
			if (!find(key))
				ArrayAllocator<T>::_insert(key);
			return *this;
		}
		set<T,_Comp>& remove(const T& key)
		{
			// détermine le pivot de suppression
			_Comp comp;
			for (size_t i=0; i < ArrayAllocator<T>::size(); ++i)
				if (comp.eq(m_Set[i],key))
				{
					ArrayAllocator<T>::_remove(i);
					break;
				}
			return *this;
		}

		/****************************************
		* intersections, unifications, exclusions
		****************************************/
		set<T,_Comp>& intersect(set<T,_Comp>& arg)
		{
			T *stack = new T[ArrayAllocator<T>::size()];
			size_t sz = 0;

			for (size_t i=0; i<ArrayAllocator<T>::size(); i++)
			{
				if (!arg.find(m_Set[i])) 
					stack[sz++] = m_Set[i];
			}

			for (size_t i=0; i<sz; i++)
				remove(stack[i]);

			delete stack;
			return *this;
		}
		set<T,_Comp>& unify(set<T,_Comp>& arg)
		{
			for (const_iterator it = arg.begin(); it != arg.end(); it++)
				insert(*it);
			return *this;
		}
		set<T,_Comp>& exclude(set<T,_Comp>& arg)
		{
			for (const_iterator it = arg.begin(); it != arg.end(); it++)
				remove(*it);
			return *this;
		}

		/****************************************
		* tests & recherches
		****************************************/
		bool find(const T& key)
		{
			_Comp comp;
			for (size_t i=0; i < ArrayAllocator<T>::size(); ++i)
				if (comp.eq(m_Set[i], key)) return true;
			return false;
		}
		bool equal(const set<T,_Comp>& arg) const 
		{ 
			for(const_iterator it = arg.begin(); it != arg.end(); it++)
				if (!find(*it)) return false;
			return true;
		}
		bool different(const set<T,_Comp>& arg) const { return !equal(arg); };

		/****************************************
		* iterateurs
		****************************************/
		const_iterator begin() { _set_iterator<T,_Comp> i; i._current = m_Set; return i; }
		const_iterator end() { _set_iterator<T,_Comp> i; i._current = m_Set + ArrayAllocator<T>::size(); return i; }

		/****************************************
		* opérateurs ensemblistes
		****************************************/
		set<T,_Comp>& operator=(const set<T,_Comp>& arg) { assign(arg); return *this; }
		set<T,_Comp>& operator&=(set<T,_Comp>& arg) { return intersect(arg); }
		set<T,_Comp>& operator|=(set<T,_Comp>& arg) { return unify(arg); }
		set<T,_Comp>& operator^=(set<T,_Comp>& arg) { return exclude(arg); }
		bool operator ==(const set<T,_Comp>& right) { return equal(right); }
		bool operator !=(const set<T,_Comp>& right) { return different(right); }
		set<T,_Comp> operator&(const set<T,_Comp>& right) { set<T,_Comp> res = *this; res &= right; return res; }
		set<T,_Comp> operator|(const set<T,_Comp>& right) { set<T,_Comp> res = *this; res |= right; return res; }
		set<T,_Comp> operator^(const set<T,_Comp>& right) { set<T,_Comp> res = *this; res ^= right; return res; }

		/****************************************
		* opérateurs élémentaires
		****************************************/
		bool operator ==(T right) { return find(right); };
		bool operator !=(T right) { return !find(right); };
		bool operator ==(const T *right) { return find(*right); }
		bool operator !=(const T *right) { return !find(*right); }
		set<T,_Comp>& operator<<(T right) { insert(right); return *this; }
		set<T,_Comp>& operator+=(T key) { insert(key); return *this; }
		set<T,_Comp>& operator-=(T key) { remove(key); return *this; }
		set<T,_Comp> operator+(T right) { set<T,_Comp> res = *this; res.insert(right); return res; }
		set<T,_Comp> operator-(T right) { set<T,_Comp> res = *this; res.remove(right); return res; }
		set<T,_Comp>& operator<<(const T *right) { insert(*right); return *this; }
		set<T,_Comp>& operator+=(const T *key) { insert(*key); return *this; }
		set<T,_Comp>& operator-=(const T *key) { remove(*key); return *this; }
		set<T,_Comp> operator+(const T* right) { set<T,_Comp> res = *this; res.insert(*right); return res; }
		set<T,_Comp> operator-(const T* right) { set<T,_Comp> res = *this; res.remove(*right); return res; }

	private:
		T* m_Set;
	};

	template < typename T, typename _Comp > class vector : public ArrayAllocator<T>
	{
	public:
		typedef _set_iterator<T,_Comp> const_iterator;

		/****************************************
		* Constructeurs / Destructeurs
		****************************************/
		vector() { _init(&m_Vector); }
		vector(vector<T,_Comp>& arg) { _init(&m_Vector); assign(arg); }
		vector(uint32 grow) : ArrayAllocator<T>(grow) { _init(&m_Vector); }

		/****************************************
		* manipulations
		****************************************/
		vector<T,_Comp>& insert(const T& key)
		{
			ArrayAllocator<T>::_insert(key);
			return *this;
		}
		vector<T,_Comp>& insert(const uint32 index, const T& key)
		{
			ArrayAllocator<T>::_insert(index, key);
		}
		vector<T,_Comp>& remove(const uint32 index)
		{
			if (index >= ArrayAllocator<T>::size()) return *this;
			ArrayAllocator<T>::_remove(index);
			return *this;
		}

		/****************************************
		* tests & recherches
		****************************************/
		const_iterator find(const T key)
		{
			_Comp comp;
			const_iterator it = end();
			for (int i=0; i < ArrayAllocator<T>::size(); ++i)
				if (comp.eq(m_Vector[i], key)) 
				{
					it._current = &m_Vector[i];
					break;
				}
			return it;
		}
		bool equal(const vector<T,_Comp>& arg) const 
		{ 
			for(const_iterator it = arg.begin(); it != arg.end(); it++)
				if (find(*it) != end()) return false;
			return true;
		}
		bool different(const vector<T,_Comp>& arg) const { return !equal(arg); };

		/****************************************
		* iterateurs
		****************************************/
		const_iterator begin() { _set_iterator<T,_Comp> i; i._current = m_Vector; return i; }
		const_iterator end() { _set_iterator<T,_Comp> i; i._current = m_Vector + ArrayAllocator<T>::size(); return i; }

		/****************************************
		* opérateurs ensemblistes
		****************************************/
		vector<T,_Comp>& operator=(const vector<T,_Comp>& right) { assign(right); return *this; }
		bool operator ==(const vector<T,_Comp>& right) { return equal(right); }
		bool operator !=(const vector<T,_Comp>& right) { return different(right); }

		/****************************************
		* opérateurs élémentaires
		****************************************/
		T& operator [](const uint32 index) { ASSERT(index >= 0 && index < ArrayAllocator<T>::size()); return m_Vector[index]; }
		bool operator ==(T right) { return find(right) != end(); };
		bool operator !=(T right) { return find(right) == end(); };
		bool operator ==(const T *right) { return find(*right) != end(); }
		bool operator !=(const T *right) { return find(*right) == end(); }
		vector<T,_Comp>& operator<<(T right) { insert(right); return *this; }
		vector<T,_Comp>& operator+=(T key) { insert(key); return *this; }
		vector<T,_Comp>& operator-=(T key) { remove(key); return *this; }
		vector<T,_Comp> operator+(T right) { vector<T,_Comp> res = *this; res.insert(right); return res; }
		vector<T,_Comp> operator-(T right) { vector<T,_Comp> res = *this; res.remove(right); return res; }
		vector<T,_Comp>& operator<<(const T *right) { insert(*right); return *this; }
		vector<T,_Comp>& operator+=(const T *key) { insert(*key); return *this; }
		vector<T,_Comp>& operator-=(const T *key) { remove(*key); return *this; }
		vector<T,_Comp> operator+(const T* right) { vector<T,_Comp> res = *this; res.insert(*right); return res; }
		vector<T,_Comp> operator-(const T* right) { vector<T,_Comp> res = *this; res.remove(*right); return res; }

	private:
		T* m_Vector;
	};

	template < typename T, typename _Comp >	set<T,_Comp>& operator>>(set<T,_Comp>& set, std::string& arg)
	{
		arg.clear();
		for (_set_iterator<T,_Comp> itr = set.begin(); itr != set.end(); ++itr)
			arg += *itr;
		return set;
	}

	template < typename T, typename _Comp >
	set<T,_Comp>& operator>>(set<T,_Comp>& set, std::list<T>& arg)
	{
		arg.clear();
		for (_set_iterator<T,_Comp> itr = set.begin(); itr != set.end(); ++itr)
			arg.push_back(*itr);
		return set;
	}

	template < typename T, typename _Comp >
	set<T,_Comp>& operator>>(set<T,_Comp>& set, std::vector<T>& arg)
	{
		arg.clear();
		for (_set_iterator<T,_Comp> itr = set.begin(); itr != set.end(); ++itr)
			arg.push_back(*itr);
		return set;
	}

	/**************************************************************************************************
	* Attention: Cette fonction n'est pas sécurisée et présente une vulnérabilité.
	* Ne l'utilisez que si vous êtes sûres que le buffer de destination est assez grand pour contenir
	* la représentation de l'ensemble.
	**************************************************************************************************/
	template < typename T, typename _Comp >
	set<T,_Comp>& operator>>(set<T,_Comp>& set, void* arg)
	{
		std::string str;
		set >> str;
		strcpy((char*)arg, str.c_str());
		return set;
	}
}

#endif
