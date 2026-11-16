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
		At	Hips
		Part	:Fire_Hellfire_Big.part
		Part	:Fire_Hellfire_Trail.part
		Part	:Sparks_Hellfire.part

		POther	Chest
	End

	Event
		Type	PositOnly
		At	Neck
		Part	:Fire_Hellfire_Trail.part
		POther	Hips
	End
End

#########################################################

End