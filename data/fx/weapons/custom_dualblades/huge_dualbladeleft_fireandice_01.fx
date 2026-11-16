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
				PositionOffset	-0.1 -0.02 0.0
				PYRRotate	0.0 0.0 180.0
			End
			Anim	CustomWeapon_BladeLeft_FireAndIce_01
		End
	End

###########################################################

End
