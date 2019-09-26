#pragma once
class Car;

/// <summary>
/// ステートパターンのステート基底クラス
/// </summary>
class CarState
{
public:
	virtual CarState* Update(Car* car) { return nullptr; };
	virtual void Enter(Car* car) {};
	virtual void Exit(Car* car) {};
};

/// <summary>
/// 地上ステート
/// </summary>
class OnGroundState : public CarState{
public:
	void Enter(Car* car) override;
	void Exit(Car* car) override;
	CarState* Update(Car* car) override;
private:
	static const int engineParam = 7000;
	const float normalMaxSpeed = 90.f;
	const float boostMaxSpeed = 100.f;
};

/// <summary>
/// 空中ステート
/// </summary>
class InAirState : public CarState {
public:
	void Enter(Car* car) override;
	void Exit(Car* car) override;
	CarState* Update(Car* car) override;
};

/// <summary>
/// フリップ中のステート
/// </summary>
class FlipState : public CarState {
public:
	void Enter(Car* car) override;
	void Exit(Car* car) override;
	CarState* Update(Car* car) override;
private:
	float m_cooltimer = 0.f; //制御回復タイマー
	const float m_flipCoolTime = 1.f; //フリップをすることで制御を失う時間(秒)
	bool m_isFirst = true;
};

/// <summary>
/// 車のブースト処理
/// </summary>
static void Boost(Car* car);