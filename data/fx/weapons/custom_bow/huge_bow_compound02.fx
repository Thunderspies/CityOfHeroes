#############################################################
## Costume Anim Entity - Bow 02
#############################################################

FxInfo

Flags InheritAlpha InheritAnimScale DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Bow\Huge_WepLScale.bhvr
		Anim	CustomWeapon_Bow_02
	End
End

Condition
	On 		Triggerbits
	Triggerbits	LEFTHAND
	DoMany		1

	Event
		EName	WeaponDrawn
		While	LEFTHAND
		Flags	OneAtATime
		Type	Local
		At	WepL
		Bhvr	WEAPONS\Custom_Bow\Huge_WepLScale.bhvr
		ChildFX	:CompoundBow_LaserSight.fx
	End
End

#############################################################

End