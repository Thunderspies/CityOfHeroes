#############################################################
## TargettingLaser1.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## LEFT EYE ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Head
		BhvrOverride
			PositionOffset	0.0 -0.1 -0.125
			Scale		1.1 1.1 1.1
		End
		Anim	Aura_CyborgCombatHUD
	End
End

#############################################################

End