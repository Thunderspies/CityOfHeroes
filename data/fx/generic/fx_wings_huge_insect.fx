#############################################################
## Header
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

## NON-FLYING ###########################################################

Condition
	On		TriggerBits
	TriggerBits
	DoMany		1

	Event
		EName 	NoFly_wings
		Type	Local
		At	Back
		Until	FLY
		Flags	OneAtATime
                BhvrOverride
			Behavior
			FadeOutLength	20
			PositionOffset	0 .3 -.27
		End
		Anim	FX_wings_INSECT
	End
End

## FLYING ###########################################################

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
		ChildFX	GENERIC\FX_wings_Bugs_blur_HUGE.fx
	End

	Event
		EName	Blur_2
		Type	local
		At	Back
		While	FLY
		Flags	OneAtATime
		ChildFX	GENERIC\FX_wings_Bugs_blur_HUGE_2.fx
	End
End

#############################################################

End