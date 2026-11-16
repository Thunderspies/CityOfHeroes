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
		Sound Resistance_Hellfire_Loop 50 50 .25
	End

	Event
		EName	HeadFX
		Type	PositOnly
		At	Head
		Part	:Steam_Hellfire_Small.part
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Steam_Hellfire_Small.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Steam_Hellfire_Small.part
		POther	Neck
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Steam_Hellfire_Small.part
		POther	UArmL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Steam_Hellfire_Small.part
		POther	UArmR
	End

	Event
		EName	HipsFX
		Type	PositOnly
		At	Hips
		Part	:Steam_Hellfire_Small.part
		POther	Chest
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	:Steam_Hellfire_Small.part
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	:Steam_Hellfire_Small.part
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
		ChildFX	:Death_Hellfire_Demonling.fx
		Lifespan 190
	End

	Event
		EName	SoundFX
		Type 	Destroy
	End

	Event
		EName	HeadFX
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
		EName	LArmLFX
		Type 	Destroy
	End

	Event
		EName	LArmRFX
		Type 	Destroy
	End

	Event
		EName	HipsFX
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