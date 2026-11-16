#########################################################
##	jack
##
FxInfo

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Create
		Inherit	Position Rotation #Scale
		Update	Position Rotation
		At	LarmL
		Bhvr	Behaviors/Weapon_Fade_UP.bhvr
		BhvrOverride
			InheritGroupTint
			PositionOffset	-.2 0 0
			pyrRotate	25 0 0 #-5
		End
		Geom	FX_GEO_Combat_Shield
	End
End

End			