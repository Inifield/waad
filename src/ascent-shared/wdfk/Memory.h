#ifndef WDFK_MEMORY_H_
#define WDFK_MEMORY_H_

namespace WDFK
{
	/// Mod�le de base pour l'allocation de m�moire pour un type T.
	/// La d�rivation ou d�finition partiel d'un allocateur peut servir � d�finir des mod�les de sp�cifications m�moires diff�rents que l'allocation
	/// dans le tas g�n�ral.
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
