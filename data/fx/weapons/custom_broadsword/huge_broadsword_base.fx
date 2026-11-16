#############################################################
## Costume Anim Entity - Broadsword - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Broadsword\Huge_WepRScale.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		ChildFX	:FX_Broadsword_Base.fx
	End
End

#############################################################

End