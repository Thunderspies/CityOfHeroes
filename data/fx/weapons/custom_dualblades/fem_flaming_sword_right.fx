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
				Scale	0.7 0.7 0.7
			End
			CHILDFX	:CHILD_Flaming_Sword.fx
		End
	End

###########################################################

End
