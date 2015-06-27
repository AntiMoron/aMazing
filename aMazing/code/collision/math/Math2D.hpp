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

		/*
		ABC����Բ��D��λ���ж�
		����ֵ����0 ��Dλ��Բ���ڲ�
		С��0 ��λ��Բ���ⲿ
		����0 ���ĵ㹲Բ
		*/
		template<typename Type>
		Type aInCircle(const aVec2_t<Type>& a,
			const aVec2_t<Type>& b, 
			const aVec2_t<Type>& c, 
			const aVec2_t<Type>& d)
		{
			return makeMatrix(a.x - d.x, a.y - d.y, aSQUARE(a.x - d.x) + aSQUARE(a.y - d.y),
				b.x - d.x, b.y - d.y, aSQUARE(b.x - d.x) + aSQUARE(b.y - d.y),
				c.x - d.x, c.y - d.y, aSQUARE(c.x - d.x) + aSQUARE(c.y - d.y)).determinant();
		}
	}
}
