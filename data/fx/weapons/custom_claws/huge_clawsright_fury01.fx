#############################################################
## Costume Anim Entity - Right Claws - 03
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
		Bhvr	WEAPONS\Custom_Claws\Huge_WepRScale.bhvr
		ChildFX	:FX_ClawsRight_02.fx
	End
End

#############################################################

End