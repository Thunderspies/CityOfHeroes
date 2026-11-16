###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################


	Condition
	On 		Time
	Time		0

		Event
			Ename SwordAnchor
			At WepR
			Type Local
			BhvrOverride
				Scale		1.2 1.2 1.2
				PositionOffset	0.1 -0.04 0.0
			End
			Anim	CustomWeapon_Broadsword_FireAndIce_01
		End
	End

###########################################################

End
