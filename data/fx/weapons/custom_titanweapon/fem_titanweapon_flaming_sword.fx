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
				Scale	0.75 0.75 0.75
			End
			CHILDFX	:CHILD_Fem_TitanWeapon_Flaming_Sword.fx
		End
	End

###########################################################

End
