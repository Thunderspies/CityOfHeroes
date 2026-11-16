#############################################################
## Tremor.fx
#############################################################

FxInfo

Lifespan 190

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	hips
		Sound tremor 100.0 100.0 0.8
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	mystic
		Bhvr	powers\firecontrol\InfernoA.bhvr
	End
End

Condition
	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Local
		At	mystic
		Part	:Inferno.part
		Part	:Infernodark.part
		Lifespan 15
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		40
			PhysForcePower		350
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

Condition
	On 	Time
	Time 	68

	Event
		EName 	Primex
		Type	Local
		At	mystic
		Bhvr	Behaviors\CameraShake05Subtle.bhvr
		Part	:InfernorockBits.part
		Part	:InfernorockBits2.part
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    behaviors\powers\firecontrol\Infernobubble4.bhvr
		Part	:InfernoYellow2.part
		Part	:InfernoYellowFlame01.part
		Part	:InfernoYellow2dark.part
		Part	:InfernoYellowFlame01dark.part
		Lifespan 35
	End

	Event
		EName 	Prime
		Type	Local
		At	mystic
		Part	:TremorLightRays.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	68

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	behaviors\BlastRingScale3.bhvr
		geom	EarthRing
		Lifespan 10
	End
End

#############################################################

End