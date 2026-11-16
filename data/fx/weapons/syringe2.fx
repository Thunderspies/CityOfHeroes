#########################################################
##	jack
##

FxInfo

Flags InheritAlpha DontSuppress

#############################################################

Condition
	On 	Time
	Time 	12

	Event

		Type	Local
		At	WepR
		Sound syringe1 30 30 .25
	End

End


Condition
	On 	Time
	Time 	0

	Event
		EName 	stungun
		Type	Local
		At	WepR
		Geom	GEO_BoostSerum
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr

		BhvrOverride
			PositionOffset	0 -.05 0 #.175 -.1 0
			pyrRotate	180 0 0
		End


	End

End

End

