#############################################################
## GlowArms.fx
#############################################################

FxInfo

Flags DontSuppress ##InheritAlpha - Don't use inheret alpha on Aura FX

## ARMS ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Part	:Entanglement_Core_Red.part
	End
End

Condition
	On	Cycle
	Time	8


	Event
		Type	Local
		At	Origin
		ChildFX	:CHILD_Entanglement_Red.fx
		Lifespan 40
	End

End

#############################################################

End