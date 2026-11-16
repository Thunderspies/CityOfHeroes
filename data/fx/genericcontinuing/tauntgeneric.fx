#############################################################
## TauntGeneric.fx
#############################################################

FxInfo
ClampMaxScale 3.5 3.5 3.5
Lifespan 15

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound generictaunthit 70 70 0.56
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
		part	:TauntShards01.part
		bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 5
	End
End

#############################################################

End