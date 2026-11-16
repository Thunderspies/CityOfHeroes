#############################################################
## Enchant_Hellfire.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	HeadFX
		Type	PositOnly
		At	Head
		Part	:HellfireBurst_ArmFire_Core.part
		Part	:Fire_Hellfire_Highlight_Small.part
		Part	:Sparks_Hellfire.part
	End

	Event
		Type	Local
		At	Chest
		Part	:HellfireBurst_ArmFire_Core.part
		Part	:Fire_Hellfire_Highlight_Small.part

	End

	Event
		Type	Local
		At	Hips
		Part	:Physics_Hellfire_Core_Subtractive.part
	End

	Event
		Type	Local
		At	LArmL
		Part	:LivingHellfire_Fire.part
		POther	UArmL
	End

	Event
		Type	Local
		At	LArmR
		Part	:LivingHellfire_Fire.part
		POther	UArmR
	End

	Event
		Type	Local
		At	WepL
		Part	:LivingHellfire_Fire.part
		POther	LArmL

	End

	Event
		Type	Local
		At	WepR
		Part	:LivingHellfire_Fire.part
		POther	LArmR
	End

	Event
		Type	Local
		At	LLegL
		Part	:LivingHellfire_Fire.part
		POther	ULegL
	End

	Event
		Type	Local
		At	LLegR
		Part	:LivingHellfire_Fire.part
		POther	ULegR
	End

	Event
		Type	Local
		At	FootL
		Part	:LivingHellfire_Fire.part
		POther	LLegL
	End

	Event
		Type	Local
		At	FootR
		Part	:LivingHellfire_Fire.part
		POther	LLegR
	End
End

Condition
	On 	DEATH

	Event
		Type 	StartPositOnly
		At	Root
		ChildFX	:Death_LivingHellfire.fx
	End

	Event
		EName	HeadFX
		Type	Destroy
	End

End

#########################################################

End
