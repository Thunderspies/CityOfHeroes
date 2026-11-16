#############################################################
## SparkleHeadHeavy_Combat.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

	Event
		ENAME	SparkleEyesLightAnchor
		Type	Local
		At	Root
		While	COMBAT
		Flags	OneAtATime
		ChildFX	:SparkleHeadHeavy.fx
	End
End

#############################################################

End