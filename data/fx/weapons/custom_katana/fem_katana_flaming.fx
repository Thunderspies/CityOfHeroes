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
				Scale	0.8 0.8 0.8
			End
			CHILDFX	:CHILD_Fem_Flaming_Sword.fx
		End
	End

###########################################################

End
