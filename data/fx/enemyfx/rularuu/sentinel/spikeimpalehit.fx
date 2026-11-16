#########################################################
##	
##

FxInfo

Input  
	InpName	chest
End

LifeSpan	60

##################################
Condition
	On 	Time
	Time 	0

	Event
		Ename	HitFx
		Type	local 
		At	chest
		
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodePurpleFaint.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeBlueFaint.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeFaint.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		LifeSpan	7

	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/SentinelQuillprojectile.bhvr
		Geom	SentinelHitSpike01
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/SentinelQuillprojectile.bhvr
		Geom	SentinelHitSpike02
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/SentinelQuillprojectile.bhvr
		Geom	SentinelHitSpike03
		
	End

	

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/SentinelQuillprojectile.bhvr
		Geom	SentinelHitSpike04
		Sound quills3 50 50 .8
	End

	
End
	

End				