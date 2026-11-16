#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress, DontInheritTexFromCostume

#############################################################

Condition
	On 	Time
	Time 	1

	Event
		Type	Local
		At	Back
		Bhvr	behaviors/WingScale_VALKYRIE_Female.bhvr
		BhvrOverride
			PositionOffset	0 .35 .1
		End
		Anim	FX_wings_VALKYRIE
	End
End

#############################################################

End