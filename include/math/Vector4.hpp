#ifndef SURVIVOR_ENGINE_MATH_VECTOR_4
#define SURVIVOR_ENGINE_MATH_VECTOR_4

#include "../Types.hpp"
//#include <stdarg.h>


namespace SE_Math
{
	template< typename T >
	class Vector4
	{
		T m_Data[4];

		public:

		Vector4<T>()
		{
			this->Set( 0, 0, 0, 0 );
		}

		Vector4<T>( const T& A, const T& B, const T& C, const T& D )
		{
			this->Set( A, B, C, D );
		}

		void Set( const T& A, const T& B, const T& C, const T& D )
		{
			m_Data[0] = A;
			m_Data[1] = B;
			m_Data[2] = C;
			m_Data[3] = D;
		}

		Vector4<T> operator +( const Vector4<T>& Arg )
		{
			Vector4<T> Ret = *this;

			Ret.m_Data[0] += Arg.m_Data[0];
			Ret.m_Data[1] += Arg.m_Data[1];
			Ret.m_Data[2] += Arg.m_Data[2];
			Ret.m_Data[3] += Arg.m_Data[3];

			return Ret;
		}

		Vector4<T> operator -( const Vector4<T>& Arg )
		{
			Vector4<T> Ret = *this;

			Ret.m_Data[0] -= Arg.m_Data[0];
			Ret.m_Data[1] -= Arg.m_Data[1];
			Ret.m_Data[2] -= Arg.m_Data[2];
			Ret.m_Data[3] -= Arg.m_Data[3];

			return Ret;
		}

		Vector4<T> operator *( const T& Arg )
		{
			Vector4<T> Ret = *this;

			Ret.m_Data[0] *= Arg;
			Ret.m_Data[1] *= Arg;
			Ret.m_Data[2] *= Arg;
			Ret.m_Data[3] *= Arg;

			return Ret;
		}

		Vector4<T> operator /( const T& Arg )
		{
			Vector4<T> Ret = *this;

			Ret.m_Data[0] /= Arg;
			Ret.m_Data[1] /= Arg;
			Ret.m_Data[2] /= Arg;
			Ret.m_Data[3] /= Arg;

			return Ret;
		}

		operator T* ()
		{
			return this->m_Data;
		}


		inline prec_float Magnitude()
		{
			return sqrt( m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] + m_Data[2] * m_Data[2] + m_Data[3] * m_Data[3] );
		}

		inline prec_float Length()
		{
			return Magnitude();
		}
	
		void Normalize()
		{
			prec_float Mag = Magnitude();
			if( Mag == 0 ) return;

			m_Data[0] /= Mag;
			m_Data[1] /= Mag;
			m_Data[2] /= Mag;
			m_Data[3] /= Mag;
		}

		inline T& x() { return m_Data[0]; }
		inline T& y() { return m_Data[1]; }
		inline T& z() { return m_Data[2]; }
		inline T& r() { return m_Data[0]; }
		inline T& g() { return m_Data[1]; }
		inline T& b() { return m_Data[2]; }
		inline T& a() { return m_Data[3]; }
	};
}
using namespace SE_Math;



#endif
