#############################################################
## Continuing FX - Taunt
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 10

#############################################################

Condition
	On	Time
	Time	0

	Event
		ENAME	STUNFX
		Type	Local
		At	Head
		part	:TauntShards01.part
		bhvr	Behaviors\GenericParticleFade.bhvr
		Lifespan 5
	End
End

#############################################################

End