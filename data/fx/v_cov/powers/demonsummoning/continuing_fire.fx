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
		At	Head
		Part	:Fire_Fire_Trail_Small.part
	End

	Event
		Type	PositOnly
		At	Chest
		Part	:Fire_Fire_Forward.part
		Part	:Fire_Fire_Trail.part
	End

	Event
		Type	PositOnly
		At	UArmR
		Part	:Fire_Fire_Trail.part
		Part	:Fire_Fire_Forward.part
		Part	:Sparks_Fire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	UArmL
		Part	:Fire_Fire_Forward.part
		Part	:Sparks_Fire.part
		POther	Neck
	End

	Event
		Type	PositOnly
		At	Hips
		Part	:Fire_Fire_Forward.part
		Part	:Fire_Fire_Trail.part
		POther	Chest
	End
End

#########################################################

End