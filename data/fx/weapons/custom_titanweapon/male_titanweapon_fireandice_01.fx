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
				PositionOffset	0.0 0.0 -0.5
				PYRRotate	0 0 0
				Scale		1 1 1
			End
			Anim	CustomWeapon_TitanWeapon_FireAndIce_01
		End
	End

###########################################################

End
