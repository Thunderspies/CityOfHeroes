#############################################################
## NovaBlast.fx
#############################################################

FxInfo

Lifespan 90

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

#############################################################

Condition
	On 	Time
	Time 	40

	Event
		EName 	Prime
		Type	Local
		At	hips
		Bhvr	Behaviors\CameraShake01.bhvr
		Part	:Inferno.part
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
		EName 	Primex
		Type	Local
		At	hips
		Part	:InfernorockBits.part
		Part	:InfernorockBits2.part
		Lifespan 27
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	powers\firecontrol\Inferno.bhvr
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr    behaviors\powers\firecontrol\Infernobubble.bhvr
		Part	:InfernoYellow2.part
		Part	:InfernoYellowFlame01.part
		Lifespan 27
	End
End

#############################################################

Condition
	On 	Time
	Time 	57

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	behaviors\BlastRingScale2a.bhvr
		geom	MegaRing2
		Lifespan  1
	End
End

#############################################################

#Condition
#	On 	Time
#	Time 	43
#
#	Event
#		EName 	Prime8
#		Type	Local
#		At	root
#		Bhvr	:NovaRing.bhvr
#		Splat	electricring1copy
#		Lifespan 10
#	End
#
#	Event
#		EName 	Prime8
#		Type	Local
#		At	root
#		Bhvr	:NovaRing.bhvr
#		Splat	electricring1copy
#		Lifespan 10
#	End
#
#	Event
#		EName 	Prime8
#		Type	Local
#		At	root
#		Bhvr	:NovaRing.bhvr
#		Splat	electricring1copy
#		Lifespan 10
#	End
#
#	Event
#		EName 	Prime8
#		Type	Local
#		At	root
#		Lifespan 10
#		Bhvr	:NovaRing.bhvr
#		Splat	thinRing
#	End
#End

#############################################################

End