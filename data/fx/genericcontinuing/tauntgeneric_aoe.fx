#############################################################
## HEADER
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5

#############################################################

#Condition
#	On	Time
#	Time	0
#
#	Event
#		Type	Local
#		At	Origin
#		Sound	generictaunthit 70 60 .56
#	End
#End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	STUNFX
		Type	Local
		At	Head
		bhvr	Behaviors\GenericParticleFade.bhvr
		part	:TauntShards01.part
		Lifespan 5
	End
End

#############################################################

End