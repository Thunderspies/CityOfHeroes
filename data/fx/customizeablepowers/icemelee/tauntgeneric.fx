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
		Type	Local
		At	Origin
		Sound generictaunthit 70 70 .56
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	STUNFX
		Type	Local
		At	Head
		#part	:TauntCircle01.part
		part	CustomizeablePowers\IceMelee\TauntShards01.part
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 5
	End
End

#############################################################

End