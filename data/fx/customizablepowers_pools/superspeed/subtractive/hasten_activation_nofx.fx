#############################################################
## AccelerateCombat.fx
#############################################################

FxInfo

#############################################################

Condition
	On	TriggerBits
	TriggerBits	NONHUMAN

	Event
		Type	Destroy
		EName	ALL
	End
End

#############################################################


Condition
	On	Time
	Time	10

	Event
		ENAME	Bubblies
		Type	Posit
		At	Neck
		Bhvr	Behaviors\GenericParticleFade.bhvr
		Sound hasten2 60 60 0.75
		LifeSpan 30
	End
End

#############################################################

End