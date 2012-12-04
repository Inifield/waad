#ifndef WDFK_LIST_H_
#define WDFK_LIST_H_

#include "Base.h"
#include "Defines.h"

#ifndef WDFK_DEPTH
#define WDFK_DEPTH	4
#endif

namespace WDFK {
	/// Forward
	template < typename T, typename _Comp = comparable<T> > class skip_list;
	template < typename T, typename _Comp = comparable<T> > class _list_iterator;

	/// Définitions
	template < typename T, typename _Comp = comparable<T> > class _list_entity : protected ArrayAllocator< _list_entity<T,_Comp>* >
	{
		friend class skip_list<T,_Comp>;
		friend class _list_iterator<T,_Comp>;
	protected:
		/****************************************
		* constructeurs / destructeurs
		****************************************/
		_list_entity() : previous(NULL), next(NULL) { _init(&next); }
		_list_entity(const _list_entity& arg) : value(arg.value) { _init(&next); assign(arg); }

		/****************************************
		* propriétés
		****************************************/
		T value;

		/****************************************
		* chaînage
		****************************************/
		_list_entity* previous;
		_list_entity** next;
	};

	template < typename T, typename _Comp > class _list_iterator
	{
		friend class skip_list<T,_Comp>;
	public:
		/****************************************
		* constructeurs / destructeurs
		****************************************/
		_list_iterator() { _current = NULL; }
		_list_iterator(const _list_iterator& arg) : _current(arg._current) { }
		~_list_iterator() { }

		/****************************************
		* modifications
		****************************************/
		_list_iterator& operator++() { _current = (_current) ? _current->next[0] : NULL; return *this; }
		_list_iterator operator++(int) { _list_iterator t = *this; ++*this; return t; }
		_list_iterator& operator--() { _current = (_current) ? _current->previous[0] : NULL; return *this; }
		_list_iterator operator--(int) { _list_iterator t = *this; --*this; return t; }
		_list_iterator& operator=(const _list_iterator& arg) { _current = arg._current; return *this; }

		/****************************************
		* tests
		****************************************/
		bool operator==(const _list_iterator& arg) const { return _current == arg._current; }
		bool operator!=(const _list_iterator& arg) const { return _current != arg._current; }

		/****************************************
		* extraction de la valeur
		****************************************/
		operator T*() { return &_current->value; }
		_list_iterator& operator >>(T& arg) { arg = _current->value; return ++*this; }

	protected:
		_list_entity<T,_Comp> *_current;
	};

	template < typename T, typename _Comp> class skip_list
	{
		typedef _list_entity<T,_Comp>* list_entity;
	public:
		typedef _list_iterator<T,_Comp> const_iterator;

		/****************************************
		* Constructeurs / Destructeurs
		****************************************/
		skip_list() : m_Depth(WDFK_DEPTH), m_Size(0), m_First(NULL), m_Last(NULL) { }
		skip_list(uint32 depth) : m_Depth(depth), m_Size(0), m_First(NULL), m_Last(NULL) { }
		skip_list(const skip_list& arg) : m_Depth(arg.m_Depth), m_Size(0), m_First(NULL), m_Last(NULL) { assign(arg); }
		~skip_list() { clear(); }

		/****************************************
		* manipulations
		****************************************/
		skip_list& assign(const skip_list& arg) 
		{
			clear();

			for(_list_entity<T,_Comp>* it = arg.m_First; it; it = it->next[0])
				_insert(it->value);

			_rebuild();
			return *this;
		}

		skip_list& insert(const T key)
		{
			_insert(key);
			_rebuild(); // reconstruit les chaînages
			return *this;
		}
		skip_list& remove(const T key)
		{
			if (empty()) return *this;
			_remove(key);
			_rebuild();
			return *this;
		}
		skip_list& clear()
		{
			if (empty()) return *this;

			_list_entity<T,_Comp>* current;
			_list_entity<T,_Comp>* next;

			for (current = m_First; current; current = next)
			{
				next = current->next[0];
				delete current;
			}

			m_First = m_Last = NULL;
			m_Size = 0;
			return *this;
		}

		/****************************************
		* tests & recherches
		****************************************/
		const_iterator find(const T key)
		{
			// Cette fonction est la clé de voute du mécanisme.
			_Comp comp;
			_list_entity<T,_Comp>* current = m_First;
			const_iterator c_i;
			while(current)
			{
				if (comp.eq(key, current->value))
				{
					c_i._current = current;
					break;
				}
				/// On recherche la chaine la plus adaptée
				/// On part de la plus grande puis on reviens vers la plus petite
				///
				int i;
				for(i = current->size()-1; i; --i)
					if (current->next[i] && comp.gtoreq(key, current->next[i]->value))
						break;
				current = current->next[i];
			}
			return c_i;
		}
		bool empty() { return !m_Size; }
		uint32 size() { return m_Size; }

		/****************************************
		* iterateurs
		****************************************/
		const_iterator begin() { const_iterator i; i._current = m_First; return i; }
		const_iterator end() { return const_iterator(); }

		/****************************************
		* opérateurs élémentaires
		****************************************/
		skip_list& operator =(const skip_list& arg) { return assign(arg); }
		bool operator ==(T right) { return find(right); };
		bool operator !=(T right) { return !find(right); };
		bool operator ==(const T *right) { return find(*right); }
		bool operator !=(const T *right) { return !find(*right); }
		skip_list& operator<<(T right) { insert(right); return *this; }
		skip_list& operator+=(T key) { insert(key); return *this; }
		skip_list& operator-=(T key) { remove(key); return *this; }
		skip_list operator+(T right) { skip_list<T,_Comp> res = *this; res.insert(right); return res; }
		skip_list operator-(T right) { skip_list<T,_Comp> res = *this; res.remove(right); return res; }
		skip_list& operator<<(const T *right) { insert(*right); return *this; }
		skip_list& operator+=(const T *key) { insert(*key); return *this; }
		skip_list& operator-=(const T *key) { remove(*key); return *this; }
		skip_list operator+(const T* right) { skip_list<T,_Comp> res = *this; res.insert(*right); return res; }
		skip_list operator-(const T* right) { skip_list<T,_Comp> res = *this; res.remove(*right); return res; }

	private:
		void _insert(const T& key)
		{
			/// A l'inverse d'une liste le chaînage doit toujours être préserver, pour empêcher le cas où les
			/// chaînages de rang 4 ne donne pas de meilleur résultat que les chaînages de rang 1. On va donc reconstruire tout le chainage.
			///
			_Comp comp;

			// on créer l'objet de liste
			_list_entity<T,_Comp>* entity = new _list_entity<T,_Comp>();
			entity->value = key;

			if (empty())
			{
				m_First = m_Last = entity;
				entity->_insert(NULL);
			}
			else
			{
				// on recherche la position d'insertion
				_list_entity<T,_Comp>* prev = NULL;
				_list_entity<T,_Comp>* current = m_First;
				while(current)
				{
					bool jump = false;
					if (comp.ltoreq(key, current->value)) 
						break;

					// On tente de passer par les chainages les plus élevés puis on reviens si nécessaire vers les plus petits
					for(int i = current->size() - 1; i; --i)
					{
						if (current->next[i] && comp.gt(key, current->next[i]->value))
						{
							current = current->next[i];
							jump = true;
							break;
						}
					}
					if (!jump)
						current = current->next[0];
				}
				prev = (current) ? current->previous : m_Last;

				// On insère l'élément
				entity->_insert(current); // next[0]
				entity->previous = prev;
				if (current) 
					current->previous = entity;
				else
					m_Last = entity;
				if (prev)
					prev->next[0] = entity;
				else
					m_First = entity;
			}
			++m_Size;
		}
		void _remove(const T& key)
		{
			// on recherche la position d'insertion
			_Comp comp;
			_list_entity<T,_Comp>* prev = NULL;
			_list_entity<T,_Comp>* next = NULL;
			_list_entity<T,_Comp>* current = m_First;
			while(current)
			{
				if (comp.ltoreq(key, current->value)) 
					break;

				bool jump = false;
				// On tente de passer par les chainages les plus élevés puis on reviens si nécessaire vers les plus petits
				for(int i = current->size() - 1; i; --i)
				{
					if (current->next[i] && comp.gtoreq(key, current->next[i]->value))
					{
						current = current->next[i];
						jump = true;
						break;
					}
				}
				if (!jump) current = current->next[0];
			}

			if (!current || !comp.eq(key, current->value)) return;

			prev = current->previous;
			next = current->next[0];

			if (prev) 
				prev->next[0] = next;
			else
				m_First = next;

			if (next)
				next->previous = prev;
			else
				m_Last = next;

			delete current;
			--m_Size;
		}
		void _rebuild()
		{
			/// On reconstruit le chaînage
			if (empty()) return;

			//_list_entity<T>* nil = NULL;
			_list_entity<T,_Comp> ref;
			
			for (uint32 i=0; i<m_Depth; ++i) 
				ref._insert(NULL);

			_list_entity<T,_Comp>* current = m_Last;
			for (int ci=m_Size-1; current; --ci)
			{
				//ref[0] = current->next[0];
				current->clear();
				int sz = m_Depth - (ci % m_Depth);
				for (int i=0; i < sz; ++i)
				{
					current->_insert(ref.next[i]);
					ref.next[i] = current;
				}
				current = current->previous;
			}
		}

	private:
		uint32 m_Depth;
		uint32 m_Size;
		_list_entity<T,_Comp>* m_First;
		_list_entity<T,_Comp>* m_Last;
	};
}

#endif
