#############################################################
## Costume Anim Entity - Left Pistol - Base
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Pistols\Female_WepLScaleAndFlip.bhvr
		BhvrOverride
			Scale		1.0 1.0 1.0
		End
		ChildFX	:FX_PistolRight_Base.fx
	End
End

#############################################################

End