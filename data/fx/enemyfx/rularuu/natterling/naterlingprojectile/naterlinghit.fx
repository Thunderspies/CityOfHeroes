#########################################################
##	Fire_Ball
##
FxInfo
LifeSpan	7

Condition
	On 	Time
	Time 	0

	Event
		Ename	HitFx
		Type	local 
		At	waist
		
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodePurple.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplodeBlue.part
		Part	EnemyFx/Rularuu/Natterling/Hold/NaterLingFleshExplode.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/streakSparks.part
		sound	natterhit1 60 50 .6
		LifeSpan	7

	End

End



End	


			