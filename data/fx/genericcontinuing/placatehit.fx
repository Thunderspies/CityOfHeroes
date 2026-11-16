#############################################################
## PlacateHit.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5
LifeSpan 40

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition

	On 	Time
	Time	0

	Event 	Type 	Local
		At 	origin
		Sound placate2 70 70 .8
	End
End

#############################################################

Condition
	On	Cycle
	Time	15

	Event
		ENAME	AfraidSkull01
		Type	Local
		At	Head
		part	:PlacateCore.part
		Bhvr	Behaviors\GenericparticleFade.bhvr
		Lifespan 15
	End

	Event
		ENAME	Rings
		Type	Local
		At	node
		Altpiv	1
		Bhvr	:NodeRotate.bhvr
		part	:PlacateStreams01.part
		part	:AfraidRings01.part
		part	:PlacateCore2.part
		part	:PlacateStreams02.part
		Lifespan 15
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	node
		Type	Local
		At	Head
		Geom	CenterDummy
	End

	Event
		ENAME	Rings
		Type	Local
		At	node
		Altpiv	1
		Bhvr	:NodeRotate.bhvr
	End
End

#############################################################

End