#############################################################
## SteamPack_SteamJets_Sputter.fx
#############################################################

FxInfo
LifeSpan 0

Flags	InheritGeoScale

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On		Triggerbits
	Triggerbits	MALE
	DoMany		1

	Event
		EName	PackOffsets
		Type	Local
		At	Origin
		WHILE	MALE
		Flags	OneAtATime
		ChildFX	:Steampunk_Backpack_Steam_MALE.fx
	End
End

Condition
	On		Triggerbits
	Triggerbits	FEMALE
	DoMany		1

	Event
		EName	PackOffsets
		Type	Local
		At	Origin
		WHILE	FEMALE
		Flags	OneAtATime
		ChildFX	:Steampunk_Backpack_Steam_FEMALE.fx
	End
End

Condition
	On		Triggerbits
	Triggerbits	HUGE
	DoMany		1

	Event
		EName	PackOffsets
		Type	Local
		At	Origin
		WHILE	HUGE
		Flags	OneAtATime
		ChildFX	:Steampunk_Backpack_Steam_HUGE.fx
	End
End

#############################################################

End