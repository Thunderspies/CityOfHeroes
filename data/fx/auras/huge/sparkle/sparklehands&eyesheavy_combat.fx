#############################################################
## SparkleHands&EyesHeavy_Combat.fx
#############################################################

FxInfo
Flags NotCompatibleWithAnimalHead DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

	Event
		ENAME	BodyAnchor
		Type	Local
		At	Root
		While	COMBAT
		Flags	OneAtATime
		ChildFX	:SparkleHands&EyesHeavy.fx
	End
End

#############################################################

End