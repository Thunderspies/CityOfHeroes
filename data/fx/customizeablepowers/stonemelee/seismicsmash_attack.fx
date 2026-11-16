#############################################################
## SeismicSmash_Attack.fx
#############################################################

FxInfo

Lifespan 90

#############################################################

Condition
	On	Time
	Time	7

	Event
		Type	Local
		At	hips
		Sound stonewhoosh2 100 100 0.8
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneMelee\stonehandsa.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneHandR
		Lifespan 35
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneMelee\stonehandsa.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneHandL
		Lifespan 35
	End
End

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		Type	start
		At	mystic
		Part	CustomizeablePowers\StoneMelee\Inferno.part
		Part	CustomizeablePowers\StoneMelee\Infernodark.part
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	mystic
		Part	CustomizeablePowers\StoneMelee\TremorLightRays2.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	mystic
		Part	CustomizeablePowers\StoneMelee\InfernorockBits.part
		Part	CustomizeablePowers\StoneMelee\InfernorockBits2.part
		Lifespan 27
	End

	Event
		EName   RadiusFxScale
		Type	start
		At	mystic
		Bhvr	CustomizeablePowers\StoneMelee\Inferno.bhvr
	End

	Event
		Type	start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\StoneMelee\Infernobubble5.bhvr
		Part	CustomizeablePowers\StoneMelee\InfernoYellow2.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellowFlame01.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellow2dark.part
		Part	CustomizeablePowers\StoneMelee\InfernoYellowFlame01dark.part
		Lifespan 27
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\StoneMelee\BlastBallScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_SesmicSmashBall
		Lifespan 13
	End
End

Condition
	On 	Time
	Time 	30

	Event
		Type	start
		At	RadiusFxScale
		bhvr	CustomizeablePowers\StoneMelee\BlastRingScaleHeight.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_MegaRing2
		Lifespan 3
	End
End

#############################################################

Condition
	On 	Time
	Time 	28

	Event
		Type	Start
		At	mystic
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		800
			PhysForcePowerJitter	100
		End
		Lifespan 1
	End
End

#############################################################

End
