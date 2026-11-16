#############################################################
## BeastlyRage_FistsAndBreath.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Cycle
	Time	60

	Event
		Type	Local
		At	Origin
		ChildFX	:Child_FistsAndBreath.fx
		Lifespan 45
	End
End

#############################################################

End