#pragma once
#include <cmath>

namespace MathUtil {

	inline const float PI = 3.14;

	inline float easeInSine(float x) {
		return 1 - cosf((x * PI) / 2);
	}

	inline float easeOutSine(float x) {
		return sinf((x * PI) / 2);
	}

	inline float easeInOutSine(float x) {
		return -(cosf(PI * x) - 1) / 2;
	}

	inline float easeInQuad(float x) {
		return x * x;
	}

	inline float easeOutQuad(float x) {
		return 1 - (1 - x) * (1 - x);
	}

	inline float easeInOutQuad(float x) {
		return x < 0.5 ? (2 * x * x) : (1 - pow(-2 * x + 2, 2) / 2);
	}

	inline float easeInCubic(float x) {
		return x * x * x;
	}

	inline float easeOutCubic(float x) {
		return 1 - pow(1 - x, 3);
	}

	inline float easeInOutCubic(float x) {
		return x < 0.5 ? (4 * x * x * x) : (1 - pow(-2 * x + 2, 3) / 2);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="start">開始</param>
	/// <param name="end">終了</param>
	/// <param name="rate">割合（0.0~1.0）</param>
	/// <param name="ease">イージング関数</param>
	template <typename T>
	T Lerp(T start, T end, float rate, float (*ease)(float x) = nullptr) {
		if (rate < 0.0f) rate = 0.0f;
		if (rate > 1.0f) rate = 1.0f;
		if (ease != nullptr) {
			rate = ease(rate);
		}
		return (end - start) * rate + start;
	}

}