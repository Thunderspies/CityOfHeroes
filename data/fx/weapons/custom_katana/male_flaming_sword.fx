###########################################################

FxInfo

	Flags InheritAlpha InheritAnimScale DontSuppress

###########################################################


	Condition
	On 		Triggerbits
	Triggerbits	RIGHTHAND
	DoMany		1


		Event
			Ename SwordAnchor
			At WepR
			WHILE	RIGHTHAND
			Flags	OneAtATime
			Type Local
			CHILDFX	:CHILD_TitanWeapon_Flaming_Sword.fx
		End

###########################################################

End
