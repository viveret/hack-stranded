#ifndef SURVIVOR_ENGINE_MATH_VECTOR_2
#define SURVIVOR_ENGINE_MATH_VECTOR_2

#include "../Types.hpp"
//#include <stdarg.h>


namespace SE_Math
{
	template< typename T >
	class Vector2
	{
		T m_Data[2];

		public:

		Vector2<T>()
		{
			this->Set( 0, 0 );
		}

		Vector2<T>( const T& A, const T& B )
		{
			this->Set( A, B );
		}

		void Set( const T& A, const T& B )
		{
			m_Data[0] = A;
			m_Data[1] = B;
		}

		Vector2<T> operator +( const Vector2<T>& Arg )
		{
			Vector2<T> Ret = *this;

			Ret.m_Data[0] += Arg.m_Data[0];
			Ret.m_Data[1] += Arg.m_Data[1];

			return Ret;
		}

		Vector2<T> operator -( const Vector2<T>& Arg )
		{
			Vector2<T> Ret = *this;

			Ret.m_Data[0] -= Arg.m_Data[0];
			Ret.m_Data[1] -= Arg.m_Data[1];

			return Ret;
		}

		Vector2<T> operator *( const T& Arg )
		{
			Vector2<T> Ret = *this;

			Ret.m_Data[0] *= Arg;
			Ret.m_Data[1] *= Arg;

			return Ret;
		}

		Vector2<T> operator /( const T& Arg )
		{
			Vector2<T> Ret = *this;

			Ret.m_Data[0] /= Arg;
			Ret.m_Data[1] /= Arg;

			return Ret;
		}

		operator T* ()
		{
			return this->m_Data;
		}


		prec_float Magnitude()
		{
			return sqrt( m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] );
		}

		inline prec_float Length()
		{
			return Magnitude();
		}
		void Normalize()
		{
			prec_float Mag = Magnitude();
		}

		inline T& x() { return m_Data[0]; }
		inline T& y() { return m_Data[1]; }
		inline T& z() { return m_Data[1]; }
	};
}
using namespace SE_Math;

#endif
