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
			CHILDFX	:CHILD_Huge_Flaming_Sword.fx
		End
	End

###########################################################

End
