#############################################################
## Desdemona_TauntAura.fx
#############################################################

FxInfo

#############################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Desdemona_TauntAura_Child.fx
		Lifespan 	20
	End
End

Condition
	On 	Cycle
	Time 	50

	Event
		Type	Local
		At	Origin
		Bhvr	behaviors\GenericParticleFade.bhvr
		ChildFX	:Desdemona_TauntAura_Child.fx
		Lifespan 	20
	End
End

##############################################################

End