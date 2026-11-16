#############################################################
## MineExplosion.fx
#############################################################

FxInfo
Lifespan	90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName   RadiusFxScale2
		Type	local
		At	Root
		Geom	RootToKneesAdjustment

	End

	Event
		EName 	Prime
		Type	Local
		At	RadiusFxScale2
		Altpiv    1

		Part2	:Inferno2.part
		Sound inferno4 80 80 .9

	End

End

Condition
	On 	Time
	Time 	2

	Event
		EName 	Primex
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		LifeSpan	10
		Part1	powers/fireControl/timebombBits.part

	End

	Event
		EName 	Primex
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		Bhvr	Behaviors/CameraShake02.bhvr

		Part	:SmokeExplosion.part
		Part	:fireExplosion2.part
		Part	:fireExplosion2flat.part

	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	powers/fireControl/Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:mineYellow2.part
		Part2	:mineFlame01.part

		LifeSpan	8

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale2
		Altpiv    1

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		800
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End

End
Condition
	On 	Time
	Time 	0
	Event
		EName 	Prime8
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		Part1   :mineflash.part
		LifeSpan	8

	End
End

Condition
	On 	Time
	Time	22

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	32

	Event
		EName 	Prime1
		Type	Destroy
	End

End

End
