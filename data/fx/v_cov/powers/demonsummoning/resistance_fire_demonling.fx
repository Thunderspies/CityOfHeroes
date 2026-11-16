#############################################################
## ToxicFire_Aura.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0


	Event
		EName	SoundFX
		Type 	Local
		At 	Root
		Sound Resistance_Fire_Loop 50 50 .25
	End

	Event
		EName	ChestFX
		Type	PositOnly
		At	Chest
		Part	:Fire_Fire_Demonling.part
		Part	:Steam_Fire_Small.part
		POther	Hips
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Fire_Fire_Small.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Fire_Fire_Small.part
		POther	Neck
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	:Fire_Fire_Small.part
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	:Fire_Fire_Small.part
	End
End
#########################################################

Condition
	On	DEATH

#	Event
#		EName	ALL
#		Type	Destroy
#	End

	Event
		Type	Start
		At	Root
		ChildFX	:Death_Fire_Demonling.fx
		Lifespan 190
	End

	Event
		EName	SoundFX
		Type 	Destroy
	End

	Event
		EName	ChestFX
		Type 	Destroy
	End

	Event
		EName	UArmLFX
		Type 	Destroy
	End

	Event
		EName	UArmRFX
		Type 	Destroy
	End

	Event
		EName	FootLFX
		Type 	Destroy
	End

	Event
		EName	FootRFX
		Type 	Destroy
	End

End

#########################################################

End