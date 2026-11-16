#############################################################
## Continuing FX - Debuff Endurance
#############################################################

FxInfo

ClampMaxScale 3.5 3.5 3.5

Lifespan 5400

#############################################################

Condition
	On	Cycle
	Time	120

	Event
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		ChildFX Genericcontinuing\DeBuffEnd.fx
		Lifespan 200
	End
End

#############################################################

End