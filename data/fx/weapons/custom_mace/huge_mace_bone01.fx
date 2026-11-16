#############################################################
## Costume Anim Entity - Mace - 09
#############################################################

FxInfo

Flags InheritAlpha ##InheritAnimScale

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepR
		Bhvr	WEAPONS\Custom_Mace\Huge_WepRScale.bhvr
		ChildFX	:FX_GiantBone_01.fx
	End
End

#############################################################

End