#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo

#########################################################
Condition
	On	Time
	Time	0

	Event
		Type	PositOnly
		At	UArmL
		Part	:Steam_Fire_Small.part
		Part	:Fire_Fire_Small.part
		Part	:Sparks_Fire.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Steam_Fire_Small.part
		Part	:Fire_Fire_Small.part
		Part	:Sparks_Fire.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	LArmL
		Part	:Fire_Fire_Small.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	LArmR
		Part	:Fire_Fire_Small.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Fire_Fire_Small.part
		POther	LArmL
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Fire_Fire_Small.part
		POther	LArmR
	End

	Event
		Type	PositOnly
		At	ULegL
		Part	:Fire_Fire_Small.part
	End

	Event
		Type	PositOnly
		At	ULegR
		Part	:Fire_Fire_Small.part
	End

	Event
		Type	PositOnly
		At	LLegL
		Part	:Fire_Fire_Small.part
		POther	ULegL
	End

	Event
		Type	PositOnly
		At	LLegR
		Part	:Fire_Fire_Small.part
		POther	ULegR
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Steam_Fire_Small.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Steam_Fire_Small.part
	End

End

#########################################################

End