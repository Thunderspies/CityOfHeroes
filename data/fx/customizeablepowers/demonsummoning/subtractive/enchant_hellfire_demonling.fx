#############################################################
## Enchant_Hellfire_Demonling.fx
#############################################################

FxInfo

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	UArmL
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Sparks_Hellfire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Sparks_Hellfire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Steam_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Fire_Hellfire_Small.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Steam_Hellfire_Small.part
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:Fire_Hellfire_Trail_Small.part
		Part	:Sparks_Hellfire.part
		POther	Chest
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Fire_Hellfire_Trail.part
		Part	:Sparks_Hellfire.part
		POther	Hips
	End

	Event
		Type	PositOnly
		At	Head
		Part	:Fire_Hellfire_Trail_Small.part
	End

	Event
		Type	PositOnly
		At	FootL
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Sparks_Hellfire.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Fire_Hellfire_Small.part
		Part	:Fire_Hellfire_Small_Alpha.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Sparks_Hellfire.part
		POther	LLegR
	End
End

#########################################################

End