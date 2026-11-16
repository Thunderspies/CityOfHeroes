#############################################################
## Generic Continuing - Confuse
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	Hairnode
		Type	Local
		At	hair
		Geom	HairToHeadAdjustment
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hairnode
		AltPiv	1
		Bhvr	Behaviors\GenericParticleFade.bhvr
		part	:ConfuseDots01.part
		part	:ConfuseDots02.part
		part	:ConfuseBubbles01.part
		part	:ConfuseStars01.part
	End
End

#############################################################

End