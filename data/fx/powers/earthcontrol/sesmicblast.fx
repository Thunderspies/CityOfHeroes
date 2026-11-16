#############################################################
## SesmicBlast.fx
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
		At	hips
	End

	Event
		Type	Local
		At	LARMR
		Bhvr 	behaviors\stonehandsa.bhvr
		geom	StoneHandR
		Lifespan 35
	End

	Event
		Type	Local
		At	LARML
		Bhvr 	behaviors\stonehandsa.bhvr
		geom	StoneHandL
		Lifespan 35
	End
End

#############################################################

Condition
	On 	Time
	Time 	23

	Event
		EName 	Prime
		Type	start
		At	mystic
		Part	:Inferno.part
		Part	:Infernodark.part
		Lifespan 15
	End
End

Condition
	On 	Time
	Time 	25

	Event
		EName 	Prime
		Type	start
		At	mystic
		Part	:TremorLightRays2.part
		Lifespan 10
	End
End

Condition
	On 	Time
	Time 	28

	Event
		EName 	Primex
		Type	start
		At	mystic
		Part	:InfernorockBits.part
		Part	:InfernorockBits2.part
		Lifespan 27
	End

	Event
		EName   RadiusFxScale
		Type	start
		At	mystic
		Bhvr	powers\firecontrol\Inferno.bhvr
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	SesmicSmashBall
		bhvr	behaviors\BlastBallScale.bhvr
		Lifespan 13
	End

	Event
		EName 	Prime1
		Type	start
		At	RadiusFxScale
		bhvr    behaviors\powers\firecontrol\Infernobubble5.bhvr
		Part	:InfernoYellow2.part
		Part	:InfernoYellowFlame01.part
		Part	:InfernoYellow2dark.part
		Part	:InfernoYellowFlame01dark.part
		Lifespan 27
	End

	Event
		Ename	ExplosiveForce
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

Condition
	On 	Time
	Time 	30

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		bhvr	behaviors\BlastRingScaleHeight.bhvr
		geom	EarthRing
		Lifespan 3
	End
End

#############################################################

End