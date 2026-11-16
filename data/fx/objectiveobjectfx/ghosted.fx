#########################################################
##	ghosted
##
FxInfo

Condition
	Event
		Type	Local
		At	Origin 
		Bhvr	:GhostedObjectiveObject.bhvr
		Flags	AdoptParentEntity
	End

	Event
		Type	local
		At	Origin
		Sound objective_loop 100 100 .8
	End
End


Condition
	On Cycle
	Time 30

	Event
		Type	SetLight
		Bhvr	:lightPulse.bhvr
		LifeSpan 30
	End
End


End			