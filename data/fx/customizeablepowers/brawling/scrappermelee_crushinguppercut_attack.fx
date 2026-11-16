#############################################################
## CrushingUpperCut_Attack.fx
#############################################################

FxInfo
Lifespan 100

#############################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup.part
		Lifespan 20
	End
End

Condition
	On	Time
	Time	20
	Event
		Type	Local
		At	WepR
		Part	:Hand_Buildup_Star.part
		Part	:Hand_Buildup_Ring.part

	End

	Event
		Type	Local
		At	WepR
		Part	:Hand_Trail.part
		Part	:Hand_Trail_Additive.part
		Lifespan 11
	End
End

Condition
	On	Time
	Time	28

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 0.0 2.0
		End
		Part	:Ground_Dust_Traveling_Small.part
		Lifespan 1
	End
End

Condition
	On	Time
	Time	29

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 -2.0 6.0
		End
		Part	:Air_Dust_Traveling_Up.part
		Part	:Air_Dust_Traveling_Up_Additive.part
		Part	:Air_Dust_Traveling_Up_Core.part
		Lifespan 1
	End
End

#############################################################

End