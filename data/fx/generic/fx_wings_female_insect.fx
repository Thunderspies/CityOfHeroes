#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 		TriggerBits
	TriggerBits	FLY
	DoMany		1

	Event
		EName	Blur_1
		Type	LocaL
		At	Back
		While	FLY
		Flags	OneAtATime
		ChildFX	GENERIC\FX_wings_Bugs_blur_FEM.fx
	End

	Event
		EName	Blur_2
		Type	local
		At	Back
		While	FLY
		Flags	OneAtATime
		ChildFX	GENERIC\FX_wings_Bugs_blur_FEM_2.fx
	End
End


#####  Basic Wings - these fade out on flight  #############

Condition
	On		TriggerBits
	TriggerBits
	DoMany		1

	Event
		EName 	NoFly_wings
		Type	Local
		At	Back
		Anim	FX_wings_INSECT
		Until	FLY
		Flags	OneAtATime
		Bhvr	behaviors/WingScale_INSECT_Female.bhvr
                BhvrOverride
			Behavior
			FadeOutLength	20
			PositionOffset	0 .1 .1
		End
	End
End


###### extra blur Wings for JUMP  #########################

Condition
	On 		TriggerBits
	TriggerBits	JUMP
	DoMany		1

	Event
		EName	Blur_1
		Type	LocaL
		At	Back
		While	JUMP
		Flags	OneAtATime
		ChildFX	GENERIC\FX_wings_Bugs_blur_FEM.fx
	End

	Event
		EName	Blur_2
		Type	local
		At	Back
		While	JUMP
		Flags	OneAtATime
		ChildFX	GENERIC\FX_wings_Bugs_blur_FEM_2.fx
	End
End


#####  Basic Wings - these fade out on JUMP  #############

Condition
	On		TriggerBits
	TriggerBits
	DoMany		1

	Event
		EName 	NoFly_wings
		Type	Local
		At	Back
		Anim	FX_wings_INSECT
		Until	JUMP
		Flags	OneAtATime
		Bhvr	behaviors/WingScale_INSECT_Female.bhvr
                BhvrOverride
			Behavior
			FadeOutLength	20
			PositionOffset	0 .1 .1
		End
	End
End


End