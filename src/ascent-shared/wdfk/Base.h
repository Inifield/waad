#ifndef WDFK_BASE_H_
#define WDFK_BASE_H_

//#include "Memory.h"

namespace WDFK 
{
	template < typename T > class ArrayAllocator
	{
	protected:
		ArrayAllocator() : m_Size(0), m_Capacity(0), m_Growth(4), m_Array(NULL)  { }
		ArrayAllocator(uint32 grow) : m_Size(0), m_Capacity(0), m_Growth(grow), m_Array(NULL)  { }
	public:
		/****************************************
		* Constructeurs / Destructeurs
		****************************************/
		virtual ~ArrayAllocator() { _clear(true); }

		/****************************************
		* manipulations
		****************************************/
		ArrayAllocator<T>& assign(const ArrayAllocator<T>& arg)
		{
			_clear(false);

			m_Size = arg.m_Size;
			m_Capacity = arg.m_Capacity;

			T* to = new T[arg.m_Capacity];
			T* from = *(arg.m_Array);

			/// On fait une copie d'élément par élément
			for(uint32 i=0; i < m_Size; ++i)
				to[i] = from[i];
			//memcpy(to, from, m_Capacity * sizeof(T));

			(*m_Array) = to;
			return *this;
		}
		void clear() { _clear(false); }

		/****************************************
		* tests & recherches
		****************************************/
		inline bool empty() { return !m_Size; }
		inline size_t size() { return m_Size; }

	protected:
		/****************************************
		* initialisation (Doit être appelée pour utiliser l'allocation)
		****************************************/
		void _init(T** var) 
		{
			m_Array = var;
			*m_Array = NULL;
		}
		inline void setGrowth(size_t grow)
		{
			m_Growth = grow;
		}

		/****************************************
		* manipulations internes
		****************************************/
		ArrayAllocator<T>& _insert(const T& key)
		{
			_increase();
			(*m_Array)[m_Size-1] = key;
			return *this;
		}
		ArrayAllocator<T>& _insert(uint32 index, const T& key)
		{
			if ((uint32)index > (uint32)m_Size)
				_increase(index - m_Size);
			else
				_increase();

			// on effectue un shift du point indiqué jusqu'à la fin avant d'insérer la donnée
			for(size_t i = index + 1; i < m_Size; ++i) (*m_Array)[i] = (*m_Array)[i-1];
			(*m_Array)[index] = key;
			return *this;
		}
		ArrayAllocator<T>& _remove(size_t index)
		{
			_decrease(index);
			return *this;
		}
	
	private:
		void _clear(bool destruct)
		{
			if (destruct)
			{
				if (m_Capacity) 
				{
					m_Capacity = m_Size = 0;
					T* arr = *m_Array;
					delete[] arr;
					*m_Array = NULL;
					//m_Array = NULL;
				}
			}
			else if (m_Size)
			{
				m_Size = 0;
				if (m_Capacity > m_Growth)
				{
					T* arr = *m_Array;
					delete[] arr;
					m_Capacity = m_Growth;
					*m_Array = new T[m_Capacity];
				}
			}
		}
		void _increase()
		{
			size_t os = m_Size;
			size_t oc = m_Capacity;
			if (++m_Size > m_Capacity)
			{
				T *old = *m_Array;
				m_Capacity += m_Growth;
				*m_Array = new T[m_Capacity];
				for(size_t i=0;i<os;++i)
					(*m_Array)[i] = old[i];
				if (os || oc)
					delete[] old;
			}
		}
		void _increase(int quantum)
		{
			while (quantum--) _increase();
		}
		void _decrease(size_t pivot)
		{
			if (--m_Size < (m_Capacity - m_Growth))
			{
				T *old = *m_Array;
				m_Capacity -= m_Growth;
				*m_Array = new T[m_Capacity];
				for(size_t i=0;i<m_Size;++i)
					(*m_Array)[i] = (i<pivot) ? old[i] : old[i+1];
				delete[] old;
			}
			else
			{
				for(size_t i=pivot;i<m_Size;++i)
					(*m_Array)[i] = (*m_Array)[i+1];
			}
		}
	private:
		size_t m_Size;
		size_t m_Capacity;
		size_t m_Growth;
		T** m_Array;
	};

	/// Racine des comparateurs de données, utilisé dans les structures de données
	template < typename T > class comparable 
	{
	public:
		comparable() { }
		virtual ~comparable() { }

		virtual bool eq(T left, T right) { return left == right; }
		virtual bool gt(T left, T right) { return left > right; }
		virtual bool gtoreq(T left, T right) { return left >= right; }
		virtual bool lt(T left, T right) { return left < right; }
		virtual bool ltoreq(T left, T right) { return left <= right; }
	};

	/// Filtre pour les hashages
	extern uint32 hash_mapper[3];

	/// Racine pour les calcul de hash_code
	template < typename T > class hashable
	{
	public:
		hashable() { }
		hashable(const hashable&) { }
		virtual ~hashable() { }

		virtual uint32 hash_code(T key)
		{
			/// dans la mesure où il est impossible d'obtenir un code de hashage pour tous les objets, on se base sur l'adresse de l'objet
			/// et sur sa taille.
			size_t sz = sizeof(T);

			uint32 c = 0;
			/// calcul d'un code de hashage universel
			for (size_t i = 0; i < sz; ++i)
			{
				c ^= hash_mapper[i % 3];
				c ^= sz << i;
				c += 61;
			}
			return c;
		}
	};

	/// Définitions de classes de bases pour certaines manipulations d'objets primitifs
	template < typename T > class PrimitiveType
	{
	public:
		virtual ~PrimitiveType<T>() { }

		virtual std::string toString() = 0;

	protected:
		PrimitiveType<T>(T arg) : m_Content(arg) { }
		T m_Content;
	};
	template <typename T> class Int : public PrimitiveType<T>
	{
	public:
		Int<T>(T arg) : PrimitiveType<T>(arg) {  }

		virtual std::string toString()
		{
			char buffer[257];

			sprintf(buffer, "%d", PrimitiveType<T>::m_Content);
			return std::string(buffer);
		}
	};
	template <typename T> class Pointer : public PrimitiveType<T>
	{
	public:
		Pointer<T>(T arg) : PrimitiveType<T>(arg) { }

		virtual std::string toString()
		{
			char buffer[257];

			sprintf(buffer, "%p", PrimitiveType<T>::m_Content);
			return std::string(buffer);
		}
	};

	/// forwardings
	template < typename K, typename V, typename _HK = hashable<K>, typename _CK = comparable<K> > class _hash_iterator;
	template < typename K, typename V, typename _HK = hashable<K>, typename _CK = comparable<K> > class _smart_iterator;

	/// utilisé par les iterateurs de dictionnaire pour faire une iteration en avant ou en arrière.
	template < typename K, typename V, typename _HK, typename _CK, typename Entry > class dictionary_base
	{
		friend class _hash_iterator< K,V,_HK,_CK >;
		friend class _smart_iterator< K,V,_HK,_CK >;
	protected:
		virtual Entry* getNextEntry(const Entry* arg) const = 0;
		virtual Entry* getPreviousEntry(const Entry* arg) const = 0;
	};
}

#endif
