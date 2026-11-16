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
				PositionOffset	0.1 -0.05 0.0
				Scale	0.7 0.7 0.7
			End
			CHILDFX	:CHILD_Flaming_Sword.fx
		End
	End

###########################################################

End
