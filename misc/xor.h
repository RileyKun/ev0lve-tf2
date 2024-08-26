#pragma once

#include <misc/rand.h>

namespace utils
{
	template<uint32_t A, uint32_t B>
	class xor_value
	{
	public:
		__forceinline static uint32_t get() { return value ^ cipher; }

	private:
		volatile static const inline uint32_t value { A ^ B }, cipher { B };
	};

	template <size_t N, char K>
	struct xor_str
	{
	private:
		static constexpr char enc(const char c) { return c ^ K; }

	public:
		template <size_t... s>
		constexpr __forceinline xor_str(const char* str, std::index_sequence<s...>)
			: encrypted { enc(str[s])... } { }

		__forceinline std::string dec()
		{
			std::string dec;
			dec.resize(N);

			for (auto i = 0u; i < N; i++)
				dec[i] = encrypted[i] ^ K;

			return dec;
		}

		__forceinline std::string ot(bool decrypt = true)
		{
			std::string dec;
			dec.resize(N);

			for (auto i = 0u; i < N; i++)
			{
				dec[i] = decrypt ? (encrypted[i] ^ K) : encrypted[i];
				encrypted[i] = '\0';
			}

			return dec;
		}

		volatile char encrypted[N];
	};

	template<typename T>
	class encrypted_ptr
	{
	public:
		__forceinline encrypted_ptr()
		{
			this->value = this->cipher = RANDOM_SEED;
		}

		__forceinline explicit encrypted_ptr(T* const value)
		{
			this->cipher = RANDOM_SEED;
			this->value = uintptr_t(value) ^ this->cipher;
		}

		__forceinline T* operator()() const
		{
			return operator->();
		}

		__forceinline T& operator*() const
	    {
	        return *reinterpret_cast<T*>(this->value ^ this->cipher);
	    }

	    __forceinline T* operator->() const
	    {
	        return reinterpret_cast<T*>(this->value ^ this->cipher);
	    }

		__forceinline bool operator==(const encrypted_ptr& other) const
		{
			return this->operator->() == other.operator->();
		}

		explicit __forceinline operator bool() const
		{
			return !this->operator!();
		}

		__forceinline bool operator!() const
		{
			return this->operator->() == nullptr;
		}

		__forceinline encrypted_ptr<T>& deref(const size_t amnt)
		{
			for (auto i = 0u; i < amnt; i++)
				*this = encrypted_ptr<T>(*reinterpret_cast<T**>(this->value ^ this->cipher));
			return *this;
		}

	private:
		volatile uintptr_t value{};
		volatile uintptr_t cipher{};
	};
}

#define NUM(val) (decltype(val))(utils::xor_value<(uint32_t) val, utils::random::_uint<__COUNTER__, 0xFFFFFFFF>::value>::get())
#define XOR_STR_S(s) utils::xor_str<sizeof(s), utils::random::_char<__COUNTER__>::value>(s, std::make_index_sequence<sizeof(s)>())
#define STR(s) XOR_STR_S(s).dec().c_str()
#define XOR_STR_OT(s) XOR_STR_S(s).ot().c_str()
#define XOR_STR_STORE(s) []() -> std::pair<std::string, char> { \
	constexpr auto key = utils::random::_char<__COUNTER__>::value; \
	return std::make_pair(utils::xor_str<sizeof(s), key>(s, std::make_index_sequence<sizeof(s)>()).ot(false), key); \
}()
#define XOR_STR_STACK(n, s) auto (n) = reinterpret_cast<char*>(alloca(((s).first.size() + 1) * sizeof(char))); \
	for (size_t i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = '\0'
#define XOR_STR_STACK_WIDE(n, s) auto (n) = reinterpret_cast<wchar_t*>(alloca(((s).first.size() + 1) * sizeof(wchar_t))); \
	for (size_t i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = '\0'
