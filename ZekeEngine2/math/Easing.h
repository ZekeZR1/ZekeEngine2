#pragma once
class EASE {
public:
	//change�͈ړ��ʁB
	//base�͍ŏ��̈ʒu�B
	//duration�͈ړ����ԁB
	//time�����݂̌o�ߎ��ԁB
	static double InQuad(double change, double base, double duration, double time) {
		time /= duration;
		return change * time * time + base;
	};
	static double OutQuad(double change, double base, double duration, double time) {
		time /= duration;
		return -change * time *(time - 2) + base;
	};
	static double InOutQuad(double change, double base, double duration, double time) {
		time /= duration / 2;
		if (time < 1) return change / 2 * time*time + base;
		return -change / 2 * ((--time)*(time - 2) - 1) + base;
	};
};