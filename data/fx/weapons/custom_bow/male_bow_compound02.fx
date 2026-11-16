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
		ChildFX	:CompoundBow_LaserSight.fx
	End
End

#############################################################

End