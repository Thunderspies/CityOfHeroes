FxInfo

#LifeSpan	240

#################################################################################

Condition

	On 	cycle
	Time 	10
	Chance	.7
	
	Event
		Type	start
		At	root

		ChildFx	V_COV/World/PortOaks/GhostSpawnContinuingFX.fx
	
		#Magnet	t_Root
		LifeSpan	30
	End


End

End