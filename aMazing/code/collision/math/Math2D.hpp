#pragma once
#include"../../engine/math/amath.hpp"


namespace aMazing
{
	namespace Math2D
	{
		/*
		����ֵ�ľ���ֵ��a,b,c���������������Ķ���
		�������0 ������ABCΪ��ʱ�룬Cλ������ֱ��AB���
		���С��0 ������ABCΪ˳ʱ�룬Cλ������ֱ��AB�Ҳ�
		�������0 �����㹲��
		*/
		template<typename Type>
		Type aOrient2D(const aVec2_t<Type>& a,
			const aVec2_t<Type>& b,
			const aVec2_t<Type>& c)
		{
			return makeMatrix<Type>(a.x - c.x, a.y - c.y,
				b.x - c.x, b.y - c.y).determinant();
		}
	}
}
