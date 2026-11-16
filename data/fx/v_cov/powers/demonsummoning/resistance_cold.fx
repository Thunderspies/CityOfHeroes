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
		EName	UArmLFX
		Type	PositOnly
		At	UArmL
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
		POther	LarmL
	End

	Event
		EName	UArmRFX
		Type	PositOnly
		At	UArmR
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
		POther	LArmR
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmL
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
		POther	HandL
	End

	Event
		EName	LArmRFX
		Type	PositOnly
		At	LArmR
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
		POther	HandR
	End

	Event
		EName	ULegLFX
		Type	PositOnly
		At	ULegL
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
	#	Part	:Mist_Cold_Trail.part
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
		Part	:Mist_Cold_Detached.part
		POther	LLegL
	End

	Event
		EName	FootRFX
		Type	PositOnly
		At	FootR
		Part	:Mist_Cold.part
		Part	:Mist_Cold_Detached.part
		POther	LLegR
	End
End

Condition
	On		DEATH

#	Event
#		EName	All
#		Type 	Destroy
#	End

	Event
		EName	DeathFX
		Type 	PositOnly
		At	Root
		ChildFX	:Death_Cold_DemonPrince.fx
		Lifespan	180
	End


	Event
		EName	SoundFX
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