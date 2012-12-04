#ifndef WDFK_MEMORY_H_
#define WDFK_MEMORY_H_

namespace WDFK
{
	/// Modèle de base pour l'allocation de mémoire pour un type T.
	/// La dérivation ou définition partiel d'un allocateur peut servir à définir des modèles de spécifications mémoires différents que l'allocation
	/// dans le tas général.
	template <typename T> class allocator
	{
	public:
		typedef size_t size_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef typename T value_type;

		/// Ne fait rien
		allocator() { }
		/// Ne fait rien
		allocator(const allocator&) { }
		/// Ne fait rien
		template <typename Other>
		allocator(const allocator<Other>&) { }

		// Ne fait rien
		allocator& operator =(const allocator&) { return *this; }
		// Ne fait rien
		template <typename Other>
		allocator<T>& operator =(const allocator<Other>&) { return *this; }

		
	};

}

#endif
