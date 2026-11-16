#############################################################
## AnimatedTail_OrganicArmor.fx
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Hips
		Bhvr	:Huge_Scale.bhvr
		// BhvrOverride
		// 	PositionOffset		0 -0.15 -0.06
		// End
		Anim	FX_PlayerTail_OrganicArmor01
	End
End

#############################################################

End