#############################################################
## Red_JacobsElectricityShield.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	30

	Event
		Type	Local
		At	Root
		ChildFx	:Child_ElectricalSurge01.fx
		LifeSpan 10
	End
End

#############################################################

End