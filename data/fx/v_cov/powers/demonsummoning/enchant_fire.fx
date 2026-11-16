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
		Part	:Sparks_Fire.part
		POther	Neck
	End


	Event
		Type	PositOnly
		At	UArmL
		Part	:Sparks_Fire.part
		Part	:Fire_Fire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Sparks_Fire.part
		Part	:Fire_Fire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Fire.part
		POther	UArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Fire.part
		POther	UArmR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Fire_Fire.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Fire_Fire.part
	End

	Event
		Type	PositOnly
		At	ULegL
		Part1	:Fire_Fire.part
	End

	Event
		Type	PositOnly
		At	ULegR
		Part	:Fire_Fire.part
	End

	Event
		Type	PositOnly
		At	LLegL
		Part	:Fire_Fire.part
		POther	ULegL
	End

	Event
		Type	PositOnly
		At	LLegR
		Part	:Fire_Fire.part
		POther	ULegR
	End

	Event
		Type	PositOnly
		At	ToeL
		Part	:Steam_Fire.part
		Part	:Fire_Fire.part
		POther	LLegL
	End

	Event
		Type	PositOnly
		At	ToeR
		Part	:Steam_Fire.part
		Part	:Fire_Fire.part
		POther	LLegR
	End
End

#########################################################

End