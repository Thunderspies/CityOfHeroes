#############################################################
## DustDevil.fx
#############################################################

FxInfo

#############################################################
Condition
	On	Time
	Time	0
	Chance 	0.1

	Event
		EName	GalesAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	50.0 0.0 50.0
		End
		ChildFX	:DustDevil_Child.fx
		Lifespan 550
	End
End

Condition
	On	Cycle
	Time	900
	Chance 	0.5

	Event
		EName	GalesAnchor
		Type	Local
		At	Origin
		BhvrOverride
			PositionOffset	50.0 0.0 50.0
		End
		ChildFX	:DustDevil_Child.fx
		Lifespan 550
	End
End

#############################################################

End

