###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################


	Condition
	On	Time
	Time	0


		Event
			Ename SwordAnchor
			At WepL
			Type Local
			BhvrOverride
				PositionOffset	0.0 0.0 -0.6
				PYRRotate	0 0 0
				Scale		.87 .87 .87
			End
			Anim	CustomWeapon_TitanWeapon_FireAndIce_01
		End
	End

###########################################################

End
