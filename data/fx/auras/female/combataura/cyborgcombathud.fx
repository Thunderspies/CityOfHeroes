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
			PositionOffset	0.0 0.04 0.025
			Scale		0.825 0.825 0.825
		End
		Anim	Aura_CyborgCombatHUD
	End
End

#############################################################

End