#############################################################
## Costume Anim Entity - Right Pistol - Base
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
		#Anim	CustomWeapon_PistolRight_Base
		ChildFX	:FX_PistolRight_Base.fx
	End
End

#############################################################

Condition
	On 	Time
	Time 	5

	Event
		Type	Local
		At	WepL
		Bhvr	Behaviors/Reverse_Wep.bhvr
		#Anim	CustomWeapon_PistolRight_Base
		ChildFX	:FX_PistolRight_Base.fx
	End
End

#############################################################

End