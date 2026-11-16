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

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime
		Type	Local
		At	hips
		Bhvr	behaviors\CameraShake01.bhvr
	#	Part2	CustomizeablePowers\EnergyBlast\Inferno.part
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
	Time 	45

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	CustomizeablePowers\EnergyBlast\Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\EnergyBlast\Infernobubble.bhvr
		Part1	:InfernoYellow2.part
		Part2	:InfernoYellowFlame01.part
		Lifespan 27
	End
End

#############################################################

Condition
	On 	Time
	Time 	43

	Event
		Type	Local
		At	Root
		Part1   CustomizeablePowers\EnergyBlast\Infernoflash.part
		Part2   CustomizeablePowers\EnergyBlast\Infernoflash2.part
		LifeSpan 10
	End
End

Condition
	On 	Time
	Time 	57

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\EnergyBlast\BlastRingScale2a.bhvr
		BhvrOverride
			TintGeom	1
		End
		geom	tintable_MegaRing2
		LifeSpan 1
	End
End


#############################################################

End
