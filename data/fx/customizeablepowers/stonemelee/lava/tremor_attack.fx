#############################################################
## Tremor_Attack.fx
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
		Bhvr	CustomizeablePowers\StoneMelee\InfernoA.bhvr
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFX	:Child_StoneFist_Activation.fx
		LifeSpan 68
	End
End

#############################################################

Condition
	On 	Time
	Time 	63

	Event
		EName 	Prime
		Type	Local
		At	mystic
		Part	CustomizeablePowers\StoneMelee\Inferno.part
		Part	CustomizeablePowers\StoneMelee\Infernodark.part
		LifeSpan 15
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
		bhvr	Behaviors\CameraShake05Subtle.bhvr
		Part	CustomizeablePowers\StoneMelee\InfernorockBits.part
		Part	CustomizeablePowers\StoneMelee\InfernorockBits2.part
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		bhvr	CustomizeablePowers\StoneMelee\Infernobubble4.bhvr
		Part	CustomizeablePowers\StoneMelee\InfernoYellow2.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellowFlame01.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellow2dark.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellowFlame01dark.part
		LifeSpan 35
	End

	Event
		EName 	Prime
		Type	Local
		At	mystic
		Part	CustomizeablePowers\StoneMelee\TremorLightRays.part
		LifeSpan 10
	End
End

#############################################################

Condition
	On 	Time
	Time 	68

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\StoneMelee\BlastRingScale3.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_MegaRing2
		Lifespan 10
	End
End

#############################################################

End