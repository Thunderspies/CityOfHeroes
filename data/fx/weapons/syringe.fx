#############################################################
## Syringe.fx
#############################################################

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	stungun
		Type	Local
		At	WepR
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		BhvrOverride
			PyrRotate	180 0 0
		End
		Geom	syringe
	End
End

#############################################################

End