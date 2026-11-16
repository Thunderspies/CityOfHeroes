
FxInfo


##################################################################

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Start
		At	Root 
		Geom	prop_Pillar_A
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End


End

Condition
	On	Death

	Event
		Type	Local 
		At	Root 
		LifeSpan 360
	End
End

End
##################################

