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
		Part	:Steam_Hellfire.part
		Part	:Steam_Hellfire_Trail.part
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Steam_Hellfire_Trail.part
		Part	:Steam_Hellfire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Steam_Hellfire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:Steam_Hellfire.part
		POther	Chest
	End
End

#########################################################

End