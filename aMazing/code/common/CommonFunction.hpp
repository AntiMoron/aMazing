#pragma once
#include"CommonDxSupport.hpp"
#include<cmath>
#include<string>

namespace aMazing
{
	extern bool isBlank(char ch);
	//��ֵval����������
	extern float increaseByScale(float val, float scale, float upperBound, float bias);
	//��ֵval������˥��
	extern float fadeByScale(float val, float scale, float lowerBound, float bias);

	//��ֵval������ֵ����
	extern float fadeByValue(float val, float decreasement, float lowerBound);

	//��ֵval����ֵ����
	extern float increaseByValue(float val, float increasement, float upperBound);
	//����ά������λ��
	extern XMFLOAT3 normalize(XMFLOAT3 src);
	//get the hash code of c-style string literal
	extern unsigned long long hashCStringLiteral(const char* lit);
}
