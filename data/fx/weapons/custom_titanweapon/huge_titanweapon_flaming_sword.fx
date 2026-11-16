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
#				Scale	2.1 2.1 2.1
#			End
			CHILDFX	:CHILD_Huge_TitanWeapon_Flaming_Sword.fx
		End
	End

###########################################################

End
