#ifndef SURVIVOR_ENGINE_MATH_VECTOR_3
#define SURVIVOR_ENGINE_MATH_VECTOR_3

#include "../Types.hpp"
#include <LinearMath/btVector3.h>
//#include <stdarg.h>


namespace SE_Math
{
	template< typename T >
	class Vector3
	{
		T m_Data[3];

		public:

		Vector3<T>()
		{
			this->Set( 0, 0, 0 );
		}

		Vector3<T>( const T& A, const T& B, const T& C )
		{
			this->Set( A, B, C );
		}

		Vector3<T>( T* Arg )
		{
			this->Set( Arg[0], Arg[1], Arg[2] );
		}

		void Set( const T& A, const T& B, const T& C )
		{
			m_Data[0] = A;
			m_Data[1] = B;
			m_Data[2] = C;
		}

		Vector3<T> operator +( const Vector3<T>& Arg )
		{
			Vector3<T> Ret = *this;

			Ret.m_Data[0] += Arg.m_Data[0];
			Ret.m_Data[1] += Arg.m_Data[1];
			Ret.m_Data[2] += Arg.m_Data[2];

			return Ret;
		}

		Vector3<T> operator -( const Vector3<T>& Arg )
		{
			Vector3<T> Ret = *this;

			Ret.m_Data[0] -= Arg.m_Data[0];
			Ret.m_Data[1] -= Arg.m_Data[1];
			Ret.m_Data[2] -= Arg.m_Data[2];

			return Ret;
		}

		Vector3<T> operator *( const T& Arg )
		{
			Vector3<T> Ret = *this;

			Ret.m_Data[0] *= Arg;
			Ret.m_Data[1] *= Arg;
			Ret.m_Data[2] *= Arg;

			return Ret;
		}

		Vector3<T> operator /( const T& Arg )
		{
			Vector3<T> Ret = *this;

			Ret.m_Data[0] /= Arg;
			Ret.m_Data[1] /= Arg;
			Ret.m_Data[2] /= Arg;

			return Ret;
		}

		operator T* ()
		{
			return this->m_Data;
		}

		operator btVector3 ()
		{
			return btVector3( m_Data[0], m_Data[1], m_Data[2] );
		}


		inline prec_float Magnitude()
		{
			return sqrt( m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] + m_Data[2] * m_Data[2] );
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
		}

		inline T& x() { return m_Data[0]; }
		inline T& y() { return m_Data[1]; }
		inline T& z() { return m_Data[2]; }
		inline T& r() { return m_Data[0]; }
		inline T& g() { return m_Data[1]; }
		inline T& b() { return m_Data[2]; }
	};
}
using namespace SE_Math;

#endif
