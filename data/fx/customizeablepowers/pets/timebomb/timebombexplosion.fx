#############################################################
## TimeBombExplosion.fx
#############################################################

FxInfo
lifespan	120

#############################################################



Condition
	On 	Time
	Time 	0

	Event
		EName   core
		Type	local
		At	Root
		Geom	gadgetHookup
		LifeSpan	125

	End

	Event
		EName   Explodez
		Type	local
		At	core
		Altpiv	1
		BhvrOverride
			PositionOffset	0 2.5 0
		End

		LifeSpan	125

	End

	Event
		EName   RadiusFxScale2
		Type	local
		At	core
		Altpiv	1
		Part1	:TimeBombInfernoFlat.part
		Geom	RootToKneesAdjustment
		LifeSpan	25

	End

	Event
		EName 	Prime
		Type	Local
		At	RadiusFxScale2
		Altpiv    1

		Part2	:TimeBombInferno.part
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
		Part1	:TimeBombBits.part

	End

	Event
		EName 	Primex
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		Bhvr	Behaviors/CameraShake01.bhvr

		Part	:SmokeExplosion.part
		Part	:fireExplosion2.part


	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root

		Bhvr	POWERS\FireControl\Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		bhvr   	 behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part

	End


	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale2
		Altpiv    1

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		30
			PhysForcePower		1000
			PhysForcePowerJitter	200
		End

		Lifespan	1

	End
End

Condition
	On 	Time
	Time 	7
	Event
		EName 	Prime8
		Type	Local
		At	RadiusFxScale2
		Altpiv    1
		Part1   :Infernoflash.part

	End
End


Condition
	On 	Time
	Time	2
	Repeat  10

	Event
		EName 	Sparkz
		Type 	Local
		At 	Explodez
		ChildFX :FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time	2
	Repeat  6

	Event
		EName 	Sparkz
		Type 	Local
		At 	Explodez
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End

	Event
		HardwareOnly
		EName 	Sparkz
		Type 	Local
		At 	Explodez
		ChildFX :FireBurstExplode.fx
		Lifespan 70
		LifespanJitter 	20
	End
End

Condition
	On 	Time
	Time	2
	Repeat  8

	Event
		EName 	Sparkz
		Type 	Local
		At 	Explodez
		ChildFX :FireBurstUP.fx
		Lifespan 70
		LifespanJitter 	20
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
