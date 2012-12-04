#ifndef WDFK_DEFINES_H_
#define WDFK_DEFINES_H_

#include "Base.h"

namespace WDFK 
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Définitions des comparateurs de base
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	template <> class comparable<std::string>
	{
	public:
		virtual bool eq(std::string left, std::string right) { return !left.compare(right); }
		virtual bool gt(std::string left, std::string right) { return left.compare(right) > 0; }
		virtual bool gtoreq(std::string left, std::string right) { return left.compare(right) >= 0; }
		virtual bool lt(std::string left, std::string right) { return left.compare(right) < 0; }
		virtual bool ltoreq(std::string left, std::string right) { return left.compare(right) <= 0; }
	};
	template <> class comparable<const char*>
	{
	public:
		virtual bool eq(const char* left, const char* right) { return !strcmp(left, right); }
		virtual bool gt(const char* left, const char* right) { return strcmp(left, right) > 0; }
		virtual bool gtoreq(const char* left, const char* right) { return strcmp(left, right) >= 0; }
		virtual bool lt(const char* left, const char* right) { return strcmp(left, right) < 0; }
		virtual bool ltoreq(const char* left, const char* right) { return strcmp(left, right) <= 0; }
	};

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// Définitions des calculs de hachage de base
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	template <> class hashable<int>
	{
	public:
		virtual uint32 hash_code(int key)
		{
			uint32 c2 = hash_mapper[0];
			uint32 arg = (uint32)key;
			arg = (arg ^ 61) ^ (arg >> 16);
			arg = arg + (arg << 3);
			arg = arg ^ (arg >> 4);
			arg = arg * c2;
			arg = arg ^ (arg >> 15);
			return key;
		}
	};
	template <> class hashable<uint32>
	{
	public:
		virtual uint32 hash_code(uint32 key)
		{
			uint32 c2 = hash_mapper[1];
			key = (key ^ 61) ^ (key >> 16);
			key = key + (key << 3);
			key = key ^ (key >> 4);
			key = key * c2;
			key = key ^ (key >> 15);
			return key;
		}
	};
	template <> class hashable<std::string>
	{
	public:
		virtual uint32 hash_code(std::string key)
		{
			uint32 c = hash_mapper[0];
			for (size_t i = 0; i < key.length(); ++i)
			{
				c ^= (key[i] << (4 % (i+1))); 
			}
			c = (c ^ 61) ^ (c >> 16);
			c = c + (c << 3);
			c = c ^ (c >> 4);
			c = c * hash_mapper[1];
			c = c ^ (c >> 15);
			return c;
		}
	};
	template <> class hashable<const char*>
	{
	public:
		virtual uint32 hash_code(const char* key)
		{
			uint32 c = hash_mapper[0];
			uint32 sz = (uint32)strlen(key);
			for (size_t i = 0; i < sz; ++i)
			{
				c ^= (key[i] << (4 % (i+1))); 
			}
			c = (c ^ 61) ^ (c >> 16);
			c = c + (c << 3);
			c = c ^ (c >> 4);
			c = c * hash_mapper[1];
			c = c ^ (c >> 15);
			return c;
		}
	};
}

#endif
