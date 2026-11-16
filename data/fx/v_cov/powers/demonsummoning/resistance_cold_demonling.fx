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
		Sound Resistance_Cold_Loop 50 50 .1
	End

	Event
		EName	ChestFX
		Type	PositOnly
		At	Chest
		Part	:Mist_Cold_Detached.part
		POther	Hips
	End

	Event
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Mist_Cold.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Mist_Cold.part
		POther	Neck
	End

	Event
		EName	ULegLFX
		Type	PositOnly
		At	ULegL
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
	End

	Event
		EName	ULegRFX
		Type	PositOnly
		At	ULegR
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	:Mist_Cold.part
		POther	LLegL
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part1	:Mist_Cold.part
		POther	LLegR
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
		ChildFX	:Death_Cold_Demonling.fx
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
		EName	ULegLFX
		Type 	Destroy
	End

	Event
		EName	ULegRFX
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