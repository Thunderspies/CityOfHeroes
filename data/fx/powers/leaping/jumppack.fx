#############################################################
## JumpPack.fx
#############################################################

FxInfo

Flags InheritAlpha

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	FootR
		Sound Superleap_loop 50.0 50.0 0.5
		bhvr	behaviors/soundFade2.bhvr
		Flags	PowerLoopingSound
		Lifespan 100
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Posit
		At	FootR
	End
End

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	MALE
	DoMany		1

	Event
		EName	BodySpines_Male
		Type	Local
		Flags	OneAtATime
		At	Root
		While	MALE
		ChildFX	:Child_JumpPack_Male.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	FEMALE
	DoMany		1

	Event
		EName	BodySpines_Female
		Type	Local
		At	Root
		Flags	OneAtATime
		While	FEMALE
		ChildFX	:Child_JumpPack_Female.fx
	End
End

Condition
	On 		TriggerBits
	TriggerBits	HUGE
	DoMany		1

	Event
		EName	BodySpines_Huge
		Type	Local
		At	Root
		Flags	OneAtATime
		While	HUGE
		ChildFX	:Child_JumpPack_Huge.fx
	End
End

#############################################################

Condition
	On	TriggerBits
	TriggerBits	CONCRETE
	TriggerBits	GRASS		#needs grass clipping fx#
	TriggerBits	GRAVEL		#needs gravel debris fx#
	TriggerBits	DIRT
	TriggerBits	WOOD
	TriggerBits	CARPET
	TriggerBits	METAL
	DoMany		1

	Event
		EName 	Prime
		Type	Local
		At	Root
		Until	AIR
		Flags	OneAtaTime
		Part	AnimatedCharacterParts\SmokeLandingLight.part
	End
End

#############################################################

End