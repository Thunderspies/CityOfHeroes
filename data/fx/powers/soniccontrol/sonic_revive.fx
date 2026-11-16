#########################################################
##	Fireweapon
########################################################
FxInfo

Lifespan	130

######  rise up    ##################################################

Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Chest
		Sound resurrect 80 80 .85
		
	End

End



Condition
	On 	Time
	Time 	0


	Event
		Type	Local 
		At	Chest
		Part3	POWERS\Ability\AbilityDots.part
		Part4	POWERS\Ability\AbilitySparklingDots.part
		Part5	POWERS\Ability\White_ContinuingRays.part
		Lifespan 50
	End

End

#######  infuse life  ##########################


Condition
	On 	Time
	Time 	50


	Event
		Type		Local 
		At		Chest
		Part	POWERS\SonicControl\PBAOEAllies\SonicHitRingCore.part
		Part	POWERS\SonicControl\PBAOEAllies\SonicHitRing.part
		LifeSpan	18
	End

End



End
##################################

