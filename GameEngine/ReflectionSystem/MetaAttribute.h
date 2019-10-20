#pragma once
#include <unordered_map>
#include <typeindex>

class MetaAttribute;

class AttribList
{
public:
	template<typename ...AttrTypes>
	AttribList(const AttrTypes &...rhs) : _heapAlloced(false)
	{
		AddAttributes(rhs...);
	}

	AttribList() : _heapAlloced(false)
	{
	}

	AttribList(const AttribList &rhs)
	{
		*this = rhs;
	}

	template<typename T>
	AttribList &operator+(const T &rhs)
	{
		_list.insert_or_assign(typeid(T).hash_code(), &rhs);
		return *this;
	}

	AttribList &operator=(const AttribList &rhs);

	template<typename T>
	AttribList(const T &rhs) : _heapAlloced(false)
	{
		_list.insert_or_assign(typeid(T).hash_code(), &rhs);
	}

	template<typename ...AttrTypes>
	void AddAttributes(const AttrTypes &...rhs)
	{
		AddFirstThenRest(rhs...);
	}

	bool has(size_t id) const
	{
		return _list.count(id);
	}

	template<typename T>
	bool has() const
	{
		return _list.count(typeid(T).hash_code());
	}

	const MetaAttribute *get(size_t id)
	{
		return _list.at(id);
	}

	template<typename T>
	const MetaAttribute *get()
	{
		return _list.at(typeid(T).hash_code());
	}
	void Clear();
	~AttribList();
private:

	template<typename T, typename ...Rest>
	void AddFirstThenRest(const T &rhs, const Rest &...rest)
	{
		_list.insert_or_assign(typeid(T).hash_code(), &rhs);
		if constexpr(sizeof...(Rest) > 0)
			AddFirstThenRest(rest...);
	}

	bool _heapAlloced;
	std::unordered_map<size_t, const MetaAttribute *> _list;
};


class MetaAttribute
{
public:
	virtual ~MetaAttribute() = default;

	virtual MetaAttribute *clone() const = 0;
};

namespace MetaAttr
{
	class PlaceHolder : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new PlaceHolder(*this);
		}
	};

	class Reflect : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new Reflect(*this);
		}
	};

	class FloatSlider : public MetaAttribute
	{
	public:
		float start;
		float end;
		FloatSlider() : start(0), end(1)
		{

		}
		FloatSlider(const FloatSlider&rhs) : start(rhs.start), end(rhs.end)
		{

		}
		FloatSlider(float _start, float _end) : start(_start), end(_end)
		{
			
		}
		MetaAttribute *clone() const override
		{
			return new FloatSlider(*this);
		}
	};

	class DoNotSerialize : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new DoNotSerialize(*this);
		}
	};

	class HideInInspector : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new HideInInspector(*this);
		}
	};

	class IsAsset : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new IsAsset(*this);
		}
	};

	class BindToLua : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new BindToLua(*this);
		}
	};

	class IsForDebug : public MetaAttribute
	{
		MetaAttribute *clone() const override
		{
			return new IsForDebug(*this);
		}
	};
}

#ifdef CLANG
#define CLANG_ATTR(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define CLANG_ATTR(...)
#endif