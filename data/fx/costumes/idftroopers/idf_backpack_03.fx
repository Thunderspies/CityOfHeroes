#############################################################
## IDF_BackPack_huge.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress DontInheritTexFromCostume

#############################################################
## IDF Backpack
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	BackPack
		Type	Local
		At	Chest
		BhvrOverride
			PositionOffset	0.0 -0.23 0.15
			PyrRotate	0.0 0.0 0.0
			Scale		1.25 1.25 1.25
		End
		Anim	FX_IDF_BackPack_03
	End
End

#############################################################

End