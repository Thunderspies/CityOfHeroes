#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	FXnode
		Type	Local
		At	Hair
		#AltPiv	1
		part	:SleepZ01.part
		Lifespan 0
	End
End

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

Condition
	On	Cycle
	Time	45

	Event
		ENAME	AfraidSkull
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
		ENAME	AfraidRings
		Type	Local
		At	Headnode
		AltPiv	1
		part	:AfraidRings01.part
		Lifespan 0

	End

	Event
		ENAME	AfraidSmallSkulls
		Type	Local
		At	Headnode
		AltPiv	1
		Bhvr	Behaviors\Spin6.bhvr
		part	:AfraidSmallSkulls01.part
		Lifespan 0
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	STUN
		Type	Local
		At	Headnode
		AltPiv	1
		part	:StunRings01.part
		part	:StunSpark01.part
		part	:StunDots01.part
		Lifespan 0
	End
End

Condition
	On	Time
	Time	0

	Event
		ENAME	CONFUSE
		Type	Local
		At	Headnode
		AltPiv	1
		part	:ConfuseDots01.part
		part	:ConfuseBubbles01.part
		Lifespan 0
	End
End

#############################################################

End