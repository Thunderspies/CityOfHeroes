#############################################################
## Peacebringer - Quantum Flight
#############################################################

FxInfo

## CONDITIONAL ###########################################################

Condition
	On	Time
	Time	5
	
	Event
		EName	EssenceBoost_ContinuingFX
		Type	Local
		At	Origin
		Until	SHAPESHIFT
		ChildFX	:QuantumFlight_Conditional.fx
	End
End

#############################################################

End				