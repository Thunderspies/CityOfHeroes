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
				Scale		0.75 0.75 0.75
			End
			Anim	CustomWeapon_BladeRight_FireAndIce_01
		End
	End

###########################################################

End
