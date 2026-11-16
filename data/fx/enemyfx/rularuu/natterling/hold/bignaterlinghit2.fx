#########################################################
##	Fire_Ball
##
FxInfo
LifeSpan	20

Condition
	On 	Time
	Time 	12

	Event
		Type	local 
		At	waist
		sound	natter7 80 50 .79
		LifeSpan	7

	End

End


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
		sound	bigboomexplo 80 50 .9
		LifeSpan	7

	End

End



End	


			