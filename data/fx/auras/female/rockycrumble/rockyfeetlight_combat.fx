#############################################################
## RockyBodyHeavy_Combat.fx
#############################################################

FxInfo
Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

#############################################################

Condition
	On		Triggerbits
	Triggerbits	COMBAT
	DoMany		1

		Event
			EName	FeetAnchor
			At	Root
			Type	Local
			While	COMBAT
			Flags	OneAtATime
			ChildFX	:RockyFeetLight.fx
		End
End

#############################################################

End