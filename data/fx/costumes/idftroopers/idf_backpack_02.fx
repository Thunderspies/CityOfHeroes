#############################################################
## IDF_BackPack_Male.fx
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
		Anim	FX_IDF_BackPack_02
	End
End

#############################################################

End