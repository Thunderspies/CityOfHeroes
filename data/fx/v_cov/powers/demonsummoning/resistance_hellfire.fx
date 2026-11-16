#############################################################
## Resistance_Hellfire.fx
#############################################################

FxInfo

#############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	SPAWNEDASPET

	Event
		Type	Local
		At	Root
		ChildFx	:RuneCircle_Demon_Spawn.fx
	End
End

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
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Fire_Hellfire_Alpha.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Fire_Hellfire_Alpha.part
		POther	Neck
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Fire_Hellfire_Alpha.part
		POther	UArmL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Fire_Hellfire_Alpha.part
		POther	UArmR
	End

	Event
		EName	HipsFX
		Type	PositOnly
		At	Chest
		Part	:Steam_Hellfire_Trail.part
		POther	Hips
	End

	Event
		EName	HipsFX
		Type	PositOnly
		At	Hips
		Part	:Fire_Hellfire_Alpha.part
		Part	:Steam_Hellfire_Trail.part
		POther	Chest
	End

	Event
		EName	LLegLFX
		Type	PositOnly
		At	LLegL
		Part	:Fire_Hellfire_Alpha.part
		POther	ULegL
	End

	Event
		EName	LLegRFX
		Type	PositOnly
		At	LLegR
		Part	:Fire_Hellfire_Alpha.part
		POther	ULegR
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	:Fire_Hellfire_Alpha.part
		POther	LLegL
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	:Fire_Hellfire_Alpha.part
		POther	LLegR

	End
End

#########################################################

Condition
	On		DEATH

	Event
		EName	All
		Type 	Destroy
	End

	Event
		Type	Start
		At	Root
		ChildFX	V_COV\Powers\DemonSummoning\Death_Demon.fx
		Lifespan 220

	End
End

#########################################################

End