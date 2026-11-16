
FxInfo


##################################
Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	AnimusArcana_Forbidden_Knowledge_Loop 100 100 .4
	End
End

Condition
	On 	Time
	Time 	0


	Event
		EName	Prop.
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset 0 3 0
		End
		Anim	FX_cot_book_single
		Bhvr	Behaviors/GenericParticleFade.bhvr
	End



End