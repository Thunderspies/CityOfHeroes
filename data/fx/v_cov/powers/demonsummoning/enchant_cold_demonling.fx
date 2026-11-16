#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Chest
		Part	:Fire_Cold_Small.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Fire_Cold_Small.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Fire_Cold_Small.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Cold_Small.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Cold_Small.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Fire_Cold_Small.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Fire_Cold_Small.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	ULegL
		Part	:Fire_Cold_Small.part
		POther	FootL
	End

	Event
		Type	PositOnly
		At	ULegR
		Part	:Fire_Cold_Small.part
		POther	FootR
	End
End

#########################################################

End