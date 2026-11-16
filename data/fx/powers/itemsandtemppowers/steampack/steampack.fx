#############################################################
## SteamPack.fx
#############################################################

FxInfo
Flags InheritAlpha

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	SteamPack_Male
		Type	Local
		Flags	OneAtATime
		At	Root
		ChildFX	:SteamPack_MALE.fx
	End

	Event
		EName	Sputter_Male
		Type	Local
		Flags	OneAtATime
		At	Root
		Until	AIR
		ChildFX	:SteamPack_SteamJets_Sputter.fx
	End

	Event
		EName	SteamJets_Male
		Type	Local
		At	Root
		Flags	OneAtATime
		While	MALE
		ChildFX	:SteamPack_SteamJets.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	SteamPack_Female
		Type	Local
		Flags	OneAtATime
		At	Root
		ChildFX	:SteamPack_FEMALE.fx
	End

	Event
		EName	Sputter_Female
		Type	Local
		Flags	OneAtATime
		At	Root
		Until	AIR
		ChildFX	:SteamPack_SteamJets_Sputter.fx
	End

	Event
		EName	SteamJets_Female
		Type	Local
		At	Root
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:SteamPack_SteamJets_Female.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	SteamPack_Huge
		Type	Local
		Flags	OneAtATime
		At	Root
		ChildFX	:SteamPack_HUGE.fx
	End

	Event
		EName	Sputter_Huge
		Type	Local
		Flags	OneAtATime
		At	Root
		Until	AIR
		ChildFX	:SteamPack_SteamJets_Sputter_HUGE.fx
	End


	Event
		EName	SteamJets_Huge
		Type	Local
		At	Root
		Flags	OneAtATime
		While	HUGE
		ChildFX	:SteamPack_SteamJets_Huge.fx
	End
End

#############################################################

Condition
	On		TRIGGERBITS
	TRIGGERBITS	Jump
	DoMany		1

	Event
		EName 	Takeoff
		Type	Start
		At	Root
		While	Jump
		Flags	OneAtaTime
		ChildFX	:SteamPack_Takeoff.fx
	End
End

#############################################################

Condition
	On		TriggerBits
	TriggerBits	CONCRETE
	TriggerBits	GRASS
	TriggerBits	GRAVEL
	TriggerBits	DIRT
	TriggerBits	WOOD
	TriggerBits	CARPET
	TriggerBits	METAL
	DoMany		1

	Event
		EName 	Impact
		Type	Start
		At	Root
		Until	AIR
		Flags	OneAtaTime
		ChildFX	:SteamPack_Impact.fx
	End

End

#############################################################

End