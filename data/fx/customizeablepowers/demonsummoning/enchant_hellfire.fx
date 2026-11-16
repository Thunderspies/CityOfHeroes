#############################################################
## Enchant_Hellfire.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Neck
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Fire_Hellfire_Highlight.part
		Part	:Sparks_Hellfire.part
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Fire_Hellfire_Highlight.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Fire_Hellfire_Highlight.part
		POther	Chest
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Fire_Hellfire.part
		Part	:Sparks_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Fire_Hellfire_Highlight.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Fire_Hellfire_Highlight.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Fire_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	FootL
		Part	:Fire_Hellfire.part
		Part	:Sparks_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
	End

	Event
		Type	PositOnly
		At	FootR
		Part	:Fire_Hellfire.part
		Part	:Sparks_Hellfire.part
		Part	:Fire_Hellfire_Highlight.part
	End
End

#########################################################

End
