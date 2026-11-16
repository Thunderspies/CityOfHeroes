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
#			BhvrOverride
#				Scale	2.0 2.0 2.0
#			End
			CHILDFX	:CHILD_TitanWeapon_Flaming_Sword.fx
		End
	End

###########################################################

End
