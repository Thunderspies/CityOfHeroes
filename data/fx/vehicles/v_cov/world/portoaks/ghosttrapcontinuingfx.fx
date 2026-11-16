FxInfo

#LifeSpan	240

#################################################################################

Condition

	On 	cycle
	Time 	10
	Chance	.5
	
	Event
		Ename	Prime
		Type	local
		At	root

		
		ChildFx  V_COV/World/PortOaks/GhostSpawnContinuingFX.fx

		LifeSpan		200
		
		
		Magnet	t_Root


	End

End

End