#ifndef _GMTK_SWIZZLE_H_
#define _GMTK_SWIZZLE_H_

#include "../util.h"
#include "vec.h"

namespace GMTK_NAMESPACE
{
	template<int a, int b, typename T>
	class swizzle2
	{
	public:
		
		using vec_t = vec<2, T>;

		static constexpr bool is_writable = (a != b);

		typedef typename std::conditional<is_writable, swizzle2<a, b, T>, struct operation_not_available>::type writable_type;

		inline T& operator[](const int i) {
			return ((T* const)this)[i];
		}

		inline const T& operator[](const int i) const {
			return ((T* const)this)[i];
		}

		#define GMTK_SWZ2_UOP(op)															\
			vec_t operator op () const														\
			{																				\
				vec<2, T> res(static_cast<T>(0));											\
				res[0] = op ((const T*)this)[a];											\
				res[1] = op ((const T*)this)[b];											\
				return res;																	\
			}

		#define GMTK_SWZ2_OP(op)															\
			template<int a2, int b2, typename T>											\
			vec_t operator op (const swizzle2<a2, b2, T> &s) const							\
			{																				\
				vec<2, T> res(static_cast<T>(0));											\
				res[0] = ((const T*)this)[a] op ((const T*)&s)[a2];							\
				res[1] = ((const T*)this)[b] op ((const T*)&s)[b2];							\
				return res;																	\
			}																				\
																							\
			vec_t operator op (const T &v) const											\
			{																				\
				vec<2, T> res(static_cast<T>(0));											\
				res[0] = ((const T*)this)[a] op v;											\
				res[1] = ((const T*)this)[b] op v;											\
				return res;																	\
			}																				\
																							\
			friend vec_t operator op (const T &l, const swizzle2<a, b, T> &r)				\
			{																				\
				vec<2, T> res(static_cast<T>(0));											\
				res[0] = l op ((const T*)r)[a];												\
				res[1] = l op ((const T*)r)[b];												\
				return res;																	\
			}

		#define GMTK_SWZ2_ROP(op)															\
			template<int a2, int b2, typename T>											\
			writable_type& operator op (const swizzle2<a2, b2, T> &s)						\
			{																				\
				const T va2 = ((const T*)&s)[a2], vb2 = ((const T*)&s)[b2];					\
				((T*)this)[a] op va2; 														\
				((T*)this)[b] op vb2;														\
				return *this;																\
			}																				\
																							\
			writable_type& operator op (const vec_t &s)										\
			{																				\
				((T*)this)[a] op s.data[0]; 												\
				((T*)this)[b] op s.data[1];													\
				return *this;																\
			}																				\
																							\
			template<int a2, int b2, typename T> 											\
			writable_type& operator op (const T &s)											\
			{																				\
				((const T*)&s)[a] op s; 													\
				((const T*)&s)[b] op s;														\
				return *this;																\
			}

		GMTK_SWZ2_UOP(-)

		GMTK_SWZ2_ROP(=)

		GMTK_SWZ2_OP(*)
		GMTK_SWZ2_OP(/)
		GMTK_SWZ2_OP(+)
		GMTK_SWZ2_OP(-)

		GMTK_SWZ2_ROP(*=)
		GMTK_SWZ2_ROP(/=)
		GMTK_SWZ2_ROP(+=)
		GMTK_SWZ2_ROP(-=)

		#undef GMTK_SWZ2_UOP
		#undef GMTK_SWZ2_OP
		#undef GMTK_SWZ2_ROP
	};

	template <int a, int b, typename T>
	inline std::ostream& operator<<(std::ostream& os, const swizzle2<a, b, T>& s)
	{
		os << "| " << s[a] << ' ' << s[b] << " |";
		return os;
	}

}



#endif //_GMTK_SWIZZLE_H_
