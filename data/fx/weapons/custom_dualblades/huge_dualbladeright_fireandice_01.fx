###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################


	Condition
	On 	Time
	Time	0


		Event
			Ename SwordAnchor
			At WepR
			Type Local
			BhvrOverride
				PositionOffset	0.1 -0.05 0.0
			End
			Anim	CustomWeapon_BladeRight_FireAndIce_01
		End
	End

###########################################################

End
