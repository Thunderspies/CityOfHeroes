#############################################################
## Costume Anim Entity - Left Claws - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Claws\Female_WepLScaleAndFlip.bhvr
		ChildFX	:FX_ClawsRight_02.fx
	End
End

#############################################################

End