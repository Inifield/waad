#ifndef WDFK_SMARTCACHE_H_
#define WDFK_SMARTCACHE_H_

#include "Base.h"
#include "Defines.h"
#include "Sets.h"
#include "Lists.h"
#include "Dictionaries.h"

namespace WDFK 
{
#ifndef WDFK_SMART_CAPACITY
#define WDFK_SMART_CAPACITY 100
#endif

	/// Forwarding
	template < typename K, typename V, typename _HK = hashable<K>, typename _CK = comparable<K> > class smart_cache;
	template < typename K, typename V, typename _HK = hashable<K>, typename _CK = comparable<K> > class _smart_entry : public _hash_entry<K,V,_HK,_CK>
	{
		friend class smart_cache<K,V,_HK,_CK>;
	protected:
		/****************************************
		* constructeurs / destructeurs
		****************************************/
		_smart_entry() : uses(0), previous(NULL), next(NULL) { }
		_smart_entry(const _keyvalue_pair<K,V,_HK,_CK>& arg) : pair(arg), uses(0), previous(NULL), next(NULL) { }
		_smart_entry(const _smart_entry& arg) : pair(arg.pair), uses(arg.uses), previous(arg.previous), next(arg.next) { }

	public:
		/****************************************
		* propriétés
		****************************************/
		_keyvalue_pair<K,V,_HK,_CK> pair;
		uint32 uses;

	protected:
		/****************************************
		* chaînage
		****************************************/
		_smart_entry* previous;
		_smart_entry* next;

		/****************************************
		* méthodes (à effectuer sur le premier noeud d'une chaine)
		****************************************/
		_smart_entry* reverse() 
		{
			// On suppose que l'on est le premier noeud de la chaîne.
			_smart_entry* root = NULL;
			_smart_entry* prev = NULL;
			_smart_entry* current = this;
			_smart_entry* next = NULL;
			while (current != NULL)
			{
				// Vérification du chaînage
				current->previous = prev; 
				// inversion du chaînage
				next = current->next;
				current->next = current->previous;
				current->previous = next;
				// changement de noeud
				if (next == NULL) root = current;
				prev = current;
				current = next;
			}
			return root;
		}
	};

	/// Iterateurs : cet iterateur peut être hérité pour une héritière spécialisante de dictionary
	template < typename K, typename V, typename _HK, typename _CK > class _smart_iterator
	{
		friend class smart_cache<K, V,_HK,_CK>;
		typedef _smart_entry< K,V,_HK,_CK > entry;
		typedef dictionary_base< K, V, _HK, _CK, _smart_entry< K,V,_HK,_CK > > base;
	public:

		/****************************************
		* constructeurs / destructeurs
		****************************************/
		_smart_iterator() : _current(NULL), _base(NULL) { }
		_smart_iterator(const _smart_iterator& arg) : _current(arg._current), _base(arg._base) { }
		virtual ~_smart_iterator() { }

		/****************************************
		* modifications
		****************************************/
		_smart_iterator& operator++() { if (_current && _base) _current = _base->getNextEntry(_current); return *this; }
		_smart_iterator operator++(int) { _smart_iterator t = *this; ++*this; return t; }
		_smart_iterator& operator--() { if (_current && _base) _current = _base->getNextEntry(_current); return *this; }
		_smart_iterator operator--(int) { _smart_iterator t = *this; --*this; return t; }
		_smart_iterator& operator=(const _smart_iterator& arg) { _current = arg._current; _base = arg._base; return *this; }

		/****************************************
		* tests
		****************************************/
		bool operator==(const _smart_iterator& arg) const { return _current == arg._current; }
		bool operator!=(const _smart_iterator& arg) const { return _current != arg._current; }
		
		/****************************************
		* extraction de la valeur
		****************************************/
		operator _keyvalue_pair<K,V,_HK,_CK>*() { return &_current->pair; }
		uint32 uses() { return _current->uses; }

	protected:
		_smart_iterator(base *arg) : _current(NULL), _base(arg) { }

	protected:
		entry *_current;
		base *_base;
	};

	/// smart cache
	template < typename K, typename V, typename _HK, typename _CK > class smart_cache : public dictionary_base< K,V,_HK,_CK,_smart_entry< K,V,_HK,_CK > >
	{
		typedef _smart_entry< K,V,_HK,_CK > entry;
	public:
		typedef _smart_iterator< K,V,_HK,_CK > const_iterator;
		typedef _keyvalue_pair< K,V,_HK,_CK > pair;

		/****************************************
		* constructeurs / destructeurs
		****************************************/
		smart_cache() : m_Size(0), m_Split(0), m_MaxSplit(1), m_MaxSize(WDFK_SMART_CAPACITY), m_Uses(0) { }
		smart_cache(const smart_cache& arg) : m_Size(0), m_Split(0), m_MaxSplit(1), m_MaxSize(WDFK_SMART_CAPACITY), m_Uses(0) { assign(arg); }
		virtual ~smart_cache() { clear(); }

		/****************************************
		* modifications
		****************************************/
		smart_cache& assign(const smart_cache& arg)
		{
			clear();
			m_Size = arg.m_Size;
			m_Split = arg.m_Split;
			m_MaxSplit = arg.m_MaxSplit;
			m_Slots = arg.m_Slots;
			m_MaxSize = arg.m_MaxSize;
			m_Uses = arg.m_Uses;

			for(size_t bucket = 0; bucket < arg.size(); ++bucket)
			{
				entry* cur_ptr = ((smart_cache)arg).m_Slots[bucket];
				while(cur_ptr != NULL)
				{
					this->insert(pair(cur_ptr->pair));
					cur_ptr = cur_ptr->next;
				}
			}

			if (size() != arg.size())
				clear();

			return *this;
		}
		smart_cache& clear()
		{
			entry *cur_ptr;
			entry *next_ptr;

			m_Split = m_Size = 0;
			m_MaxSplit = 1;
			
			for (size_t i = 0; i < m_Slots.size(); ++i)
			{
				cur_ptr = m_Slots[i];
				while (cur_ptr != NULL)
				{
					next_ptr = cur_ptr->next;
					delete cur_ptr;
					cur_ptr = next_ptr;
				}
			}

			m_Slots.clear();
			return *this;
		}
		smart_cache& insert(const pair& arg)
		{
			if(m_Size >= m_MaxSize)
			{
				/// La taille limite est atteinte, on recherche l'élément avec les statistiques les plus faibles
				/// on le supprime et on effectue l'insertion normallement.
				entry* l_ptr = NULL;
				double l_stat;
				for (const_iterator it = begin(); it != end(); ++it)
				{
					double c_stat = _getStat(it._current);
					if (it == begin() || c_stat < l_stat)
					{
						l_ptr = it._current;
						l_stat = c_stat;
					}
				}

				/// on a le premier élément avec le taux d'utilisation le plus faible
				/// on peut le supprimer
				remove(l_ptr->pair.key);
			}

			m_Slots.insert(NULL);
			if(m_Slots.size() <= m_Size) return *this;

			size_t bucket = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg.key));

			entry* cur_ptr = m_Slots[bucket];
			entry* new_ptr;

			if(cur_ptr == NULL || _ck.lt(arg.key, cur_ptr->pair.key))
			{
				new_ptr = new entry(arg);
				new_ptr->next = cur_ptr;
				if (cur_ptr != NULL) cur_ptr->previous = new_ptr;
				m_Slots[bucket] = new_ptr;
				++m_Size;
				splitBucket();
			}
			else if(_ck.eq(arg.key, cur_ptr->pair.key))
			{
				m_Slots.remove(m_Slots.size()-1);
			}
			else
			{
				entry* prev_ptr;
				for(;;)
				{
					prev_ptr = cur_ptr;
					cur_ptr = cur_ptr->next;
					if (cur_ptr == NULL || _ck.lt(arg.key, cur_ptr->pair.key))
					{
						new_ptr = new entry(arg);
						prev_ptr->next = new_ptr;
						new_ptr->previous = prev_ptr;
						++m_Size;
						splitBucket();
						break;
					}
					else if (_ck.eq(arg.key, cur_ptr->pair.key))
					{
						m_Slots.remove(m_Slots.size()-1);
						break;
					}
				}
			}

			return *this;
		}
		smart_cache& insert(const K& key, const V& value)
		{
			return insert(make_pair<K,V,_HK,_CK>(key, value));
		}
		smart_cache& remove(const K arg)
		{
			if (empty()) return *this;

			size_t bucket = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg));
			entry *cur_ptr = m_Slots[bucket];

			if (cur_ptr != NULL)
			{
				if (_ck.eq(arg, cur_ptr->pair.key))
				{
					m_Slots[bucket] = cur_ptr->next;
					if (cur_ptr->next != NULL) cur_ptr->next->previous = NULL;
					--m_Size;
					mergeBucket();
					/// Décrémenter la taille de slot de 1
					m_Slots.remove(m_Slots.size()-1);
					delete cur_ptr;
					return *this;
				}
				entry* prev_ptr = cur_ptr;
				cur_ptr = cur_ptr->next;
				while(cur_ptr != NULL)
				{
					if (_ck.eq(arg, cur_ptr->pair.key))
					{
						prev_ptr->next = cur_ptr->next;
						if(cur_ptr->next != NULL) cur_ptr->next->previous = prev_ptr;
						--m_Size;
						mergeBucket();
						m_Slots.remove(m_Slots.size()-1);
						delete cur_ptr;
						return *this;
					}
					prev_ptr = cur_ptr;
					cur_ptr = cur_ptr->next;
				}
			}

			return *this;
		}

		/****************************************
		* recherche et tests
		****************************************/
		const_iterator find(const K& arg)
		{
			const_iterator it = const_iterator(this);
			it._current = ((smart_cache<K,V,_HK,_CK>*)this)->_get(arg);
			if (it._current != NULL) ++(it._current->uses);
			++m_Uses;
			return it;
		}

		bool has(const K& arg) const
		{
			if (empty()) return false;

			size_t bucket_num = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg));

			entry *cur_link = m_Slots[bucket_num];

			while (cur_link != NULL)
			{
				if (_ck.eq(arg, cur_link->pair.key))
					return true;
				cur_link = cur_link->next; // travel down the chain
			}

			return false;
		}

		V& operator [](const K& arg) 
		{ 
			static V dummy;
			entry* cur = ((smart_cache<K,V,_HK,_CK>*)this)->_get(arg); 
			++(cur->uses);
			++m_Uses;
			return (cur != NULL) ? cur->pair.value : dummy;
		}

		double getStat(const K& arg) const
		{
			entry* cur = ((smart_cache<K,V,_HK,_CK>*)this)->_get(arg); 
			return _getStat(cur);
		}

		bool empty() const { return !m_Size; }
		size_t size() const { return m_Size; }
		uint32 uses() const { return m_Uses; }

		/****************************************
		* iterateurs
		****************************************/
		const_iterator begin()
		{
			const_iterator it = const_iterator(this);

			if (!empty())
				/// on cherche la valeur
				it._current = m_Slots[0];

			return it;
		}
		const_iterator end()
		{
			return const_iterator(this);
		}

	protected:
		double _getStat(entry* ptr)
		{
			return (ptr != NULL) ? (double)ptr->uses / (double)m_Uses : 0;
		}
		entry* _get(const K& arg)
		{
			if (empty()) return NULL;

			size_t bucket = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg));
			entry *cur_link = m_Slots[bucket];

			while(cur_link != NULL)
			{
				if(_ck.eq(arg, cur_link->pair.key))
					return cur_link;
				cur_link = cur_link->next;
			}

			return NULL; /// pas trouvé
		}
		/// Utilisé par les iterateurs pour passer à l'élément suivant
		virtual entry* getNextEntry(const entry* arg) const
		{
			_HK _hk;
			_CK _ck;
			if(empty() || arg == NULL) return NULL;
			size_t bucket = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg->pair.key));
			entry* cur_link = ((smart_cache<K,V,_HK,_CK>*)this)->m_Slots[bucket];
			while(cur_link != NULL)
			{
				if (_ck.eq(arg->pair.key, cur_link->pair.key))
				{
					// on a trouvé la clé
					cur_link = cur_link->next;
					if (cur_link == NULL)
					{
						do
						{
							if (++bucket == m_Size) return NULL;
							cur_link = ((smart_cache<K,V,_HK,_CK>*)this)->m_Slots[bucket];
						} while (cur_link == NULL);
					}
					return cur_link;
				}
				cur_link = cur_link->next;
			}
			return NULL;
		}
		/// Utilisé par les iterateurs pour passer à l'élément précédent
		virtual entry* getPreviousEntry(const entry* arg) const
		{
			_HK _hk;
			_CK _ck;
			if(empty() || arg == NULL) return NULL;
			size_t bucket = bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(arg->pair.key));
			entry* cur_link = ((smart_cache<K,V,_HK,_CK>*)this)->m_Slots[bucket];
			while(cur_link != NULL)
			{
				if (_ck.eq(arg->pair.key, cur_link->pair.key))
				{
					// on a trouvé la clé
					cur_link = cur_link->previous;
					if (cur_link == NULL)
					{
						do
						{
							if (--bucket == m_Size) return NULL;
							cur_link = ((smart_cache<K,V,_HK,_CK>*)this)->m_Slots[bucket];
						} while (cur_link == NULL);
					}
					return cur_link;
				}
				cur_link = cur_link->previous;
			}
			return NULL;
		}
		/// Détermine le slot à utiliser
		size_t bucketOffset(size_t maxsplit, size_t splitptr, size_t hashval) const
		{
			return (hashval % maxsplit < splitptr) ?
				hashval % (maxsplit << 1) :
				hashval % maxsplit;
		}
		/// Sépare un slot
		void splitBucket()
		{
			entry *old_chain = NULL;
			entry *new_chain = NULL;
			entry *cur_ptr = NULL;
			entry *next_ptr = NULL;
			entry *prev_ptr = NULL;
			
			size_t old_place = m_Split;
			size_t new_place = old_place + m_MaxSplit;

			if (old_place + 1 >= m_MaxSplit)
			{
				if (m_Size < 2) return;
				m_MaxSplit <<= 1;
				m_Split = 0;
			}
			else
				m_Split = old_place + 1;

			/* séparation dans old_chain et new_chain */
			cur_ptr = m_Slots[old_place];

			while(cur_ptr != NULL)
			{
				next_ptr = cur_ptr->next;
				cur_ptr->previous = prev_ptr;

				if (bucketOffset(m_MaxSplit, m_Split, _hk.hash_code(cur_ptr->pair.key)) == old_place)
				{
					cur_ptr->next = old_chain;
					if (old_chain != NULL) old_chain->previous = cur_ptr; /// Préservation du chaînage de reversion
					old_chain = cur_ptr;
				}
				else
				{
					cur_ptr->next = new_chain;
					if (new_chain != NULL) new_chain->previous = cur_ptr;
					new_chain = cur_ptr;
				}

				prev_ptr = cur_ptr;
				cur_ptr = next_ptr;
			}

			/* la division inverse les chaînes, on doit donc les renverser */
			if(old_chain != NULL)
				m_Slots[old_place] = old_chain->reverse();
			else
				m_Slots[old_place] = NULL;

			if(new_chain != NULL)
				m_Slots[new_place] = new_chain->reverse();
			else
				m_Slots[new_place] = NULL;
		}
		/// Fusionne deux slots
		void mergeBucket()
		{
			if (m_Split == 0)
			{
				if (m_Size < 1)
					return; // inutile de fusionner si la taille est inférieur à 1

				m_MaxSplit >>=  1; // division par 2 du maxsplit
				m_Split = m_MaxSplit - 1;
			}
			else
				--m_Split;

			size_t new_place = m_Split;
			size_t old_place = new_place + m_MaxSplit;
			char use_old_chain;

			entry *old_chain = m_Slots[old_place];
			entry *new_chain = m_Slots[new_place];
			entry *merge_list = NULL;

			while ((old_chain != NULL) || (new_chain != NULL))
			{
				/* temps que la chaine n'est pas vide */

				if (old_chain == NULL)
				{
					/* old_chain est vide, on utilise new_chain */
					use_old_chain = 0;
				}
				else if (new_chain == NULL)
				{
					/* new_chain est vide, on utilise old_chain */
					use_old_chain = 1;
				}
				else
				{
					/* les deux chaines sont vides */
					/* si l'ancienne chaine est inférieure, utiliser l'ancienne chaîne */
					use_old_chain = _ck.lt(old_chain->pair.key, new_chain->pair.key);
				}

				if (use_old_chain)
				{
					entry *old_chain_next = old_chain->next;
					old_chain->next = merge_list;
					merge_list = old_chain;
					old_chain = old_chain_next;
				}
				else
				{
					entry *new_chain_next = new_chain->next;
					new_chain->next = merge_list;
					merge_list = new_chain;
					new_chain = new_chain_next;
				}
			}

			/* la fusion inverse l'ordre des éléments, il faut les réordonner. */
			if (merge_list != NULL)
				m_Slots[new_place] = merge_list->reverse();
			else
				m_Slots[new_place] = NULL;

			/* l'ancien slot n'a plus d'éléments */
			m_Slots[old_place] = NULL;
		}
	protected:
		size_t m_Size;
		size_t m_Split;
		size_t m_MaxSplit;
		size_t m_MaxSize;
		size_t m_Uses;
		WDFK::vector< entry* > m_Slots; 
		_HK _hk;
		_CK _ck;
	};
}

#endif
