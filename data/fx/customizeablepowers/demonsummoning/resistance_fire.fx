#############################################################
## Resistance_Fire.fx
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
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	V_COV\Powers\DemonSummoning\Steam_Fire.part
		Part	V_COV\Powers\DemonSummoning\Fire_Fire_Core.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	V_COV\Powers\DemonSummoning\Steam_Fire.part
		Part	V_COV\Powers\DemonSummoning\Fire_Fire_Core.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	Hips
		Part	V_COV\Powers\DemonSummoning\Fire_Fire_Core.part
		POther	Chest
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	V_COV\Powers\DemonSummoning\Steam_Fire.part
		Part	V_COV\Powers\DemonSummoning\Fire_Fire_Core.part
		POther	UArmL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	V_COV\Powers\DemonSummoning\Steam_Fire.part
		Part	V_COV\Powers\DemonSummoning\Fire_Fire_Core.part
		POther	UArmR
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	V_COV\Powers\DemonSummoning\Fire_Fire.part
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	V_COV\Powers\DemonSummoning\Fire_Fire.part
	End
End

#########################################################

Condition
	On		DEATH

	Event
		EName	All
		Type	Destroy
	End

	Event
		Type	Start
		At	Root
		ChildFX	:Death_Demon.fx
		Lifespan 220
	End
End

#########################################################

End