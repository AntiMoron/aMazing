#pragma once
#include"CommonDxSupport.hpp"
#include<cmath>
#include<string>

namespace aMazing
{
	bool isBlank(char ch);
	//��ֵval����������
	float increaseByScale(float val, float scale, float upperBound, float bias);
	//��ֵval������˥��
	float fadeByScale(float val, float scale, float lowerBound, float bias);

	//��ֵval������ֵ����
	float fadeByValue(float val, float decreasement, float lowerBound);

	//��ֵval����ֵ����
	float increaseByValue(float val, float increasement, float upperBound);
	//����ά������λ��
	XMFLOAT3 normalize(XMFLOAT3 src);
}
