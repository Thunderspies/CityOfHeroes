#############################################################
## Costume Anim Entity - Axe - 02
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Axe\Huge_WepRScale.bhvr
		ChildFX	:FX_Pickaxe_01.fx
	End
End

#############################################################

End