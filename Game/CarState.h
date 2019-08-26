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
	static const int engineParam = 1000;
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
private:
	bool m_isfripped = false;
	float m_cooltimer = 0.f;
	float m_flipCoolTime = 1.f; //フリップをすることで制御を失う時間(秒)
};