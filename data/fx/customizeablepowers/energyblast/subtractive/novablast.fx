#############################################################
## NovaBlast.fx
#############################################################

FxInfo

lifespan 90

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hips
		Sound Nova 200.0 200.0 0.8
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan 50
	End
End

Condition

	On 	Time
	Time 	55

	Event
		EName 	Prime
		Type	Local
		At	hips
		Bhvr	behaviors\CameraShake01.bhvr
		Part2	:Inferno.part
		Part2	:Inferno_Alpha.part

		Lifespan 32
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	60

	Event
		EName 	Primex
		Type	Local
		At	hips
		Part1	:InfernorockBits.part
		Part2	:InfernorockBits2.part
		Lifespan 27
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	:Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		Lifespan 27
	End
End

#############################################################

Condition
	On 	Time
	Time 	58

	Event
		Type	Local
		At	Root
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	62

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	:BlastRingScale2a.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	Subtractive_MegaRing2
		LifeSpan 1
	End
End

#############################################################

End
