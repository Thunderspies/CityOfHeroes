###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################


	Condition
	On 	Time
	Time	0


		Event
			Ename SwordAnchor
			At WepL
			Type Local
			BhvrOverride
				Scale		0.75 0.75 0.75
				PYRRotate	0.0 0.0 180.0
				PositionOffset	0.125 0.0 0.0
			End
			Anim	CustomWeapon_BladeLeft_FireAndIce_01
		End
	End

###########################################################

End
