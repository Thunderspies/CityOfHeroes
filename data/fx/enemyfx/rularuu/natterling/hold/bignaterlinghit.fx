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
		
		Part	:MergeLRGNaterLingFleshExplodePurple.part
		Part	:MergeLRGNaterLingFleshExplodeBlue.part
		Part	:MergeLRGNaterLingFleshExplode.part
		Part	:MergeSMLNaterLingFleshExplodeBlue.part
		Part	:MergeSMLNaterLingFleshExplodePurple.part
		Part	:MergeLRGNaterLingFleshExplodeBlue.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/Crystalsparks.part
		Part	EnemyFx/Rularuu/Natterling/Hold/streakSparks.part
		Part	:LRGNaterLingSmokeExplode.part
		sound	crystalhit 60 50 .6
		LifeSpan	7

	End

End



End	


			