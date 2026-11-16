#########################################################
##	
##
FxInfo

##########################################################

Condition
				
	Event
		Type	start 
		At	root
		BHVR	:AOEShieldRepulse.bhvr
		geom	EnergyShieldRepulse
		LifeSpan	20
	End
	
End

Condition
	On	Cycle
	Time	30
	Chance	.9

	Event
		Type	start 
		At	root
		BHVR	:AOEShieldRepulse.bhvr
		geom	EnergyShieldRepulse
		LifeSpan	10
	End
	
End

End

