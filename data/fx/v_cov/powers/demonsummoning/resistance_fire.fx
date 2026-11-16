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
		Part	:Steam_Fire.part
		Part	:Fire_Fire_Core.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Steam_Fire.part
		Part	:Fire_Fire_Core.part
		POther	Neck
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	Hips
		Part	:Fire_Fire_Core.part
		POther	Chest
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Steam_Fire.part
		Part	:Fire_Fire_Core.part
		POther	UArmL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Steam_Fire.part
		Part	:Fire_Fire_Core.part
		POther	UArmR
	End

	Event
		EName	FootLFX
		Type	PositOnly
		At	FootL
		Part	:Fire_Fire.part
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	:Fire_Fire.part
	End
End

#########################################################

Condition
	On	DEATH

	Event
		EName	ALL
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