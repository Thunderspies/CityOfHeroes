#############################################################
## Continuing FX - Fear
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition

	On 	Time
	Time	0

	Event 	Type 	Local
		At 	origin
		Sound Darkfear2_loop 70 70 .3
		bhvr	behaviors\soundFade3.bhvr
		Flags	PowerLoopingSound
		Lifespan 370
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Eyenode
		Type	Local
		At	hair
		Geom	HairToEyesAdjustment
	End

	Event
		ENAME	Headnode
		Type	Local
		At	hair
		Geom	HairToHeadAdjustment
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	AfraidSkull01
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidSkull01.part
		Bhvr	Behaviors\GenericparticleFade.bhvr
		Lifespan 45
	End
End

#############################################################

Condition
	On	Cycle
	Time	45

	Event
		ENAME	AfraidSkull01
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidSkull01.part
		Bhvr	Behaviors\GenericparticleFade.bhvr
		Lifespan 15
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	Rings
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidRings01.part
		part	:AfraidRings02.part
		Lifespan 0
	End
End

#############################################################

End