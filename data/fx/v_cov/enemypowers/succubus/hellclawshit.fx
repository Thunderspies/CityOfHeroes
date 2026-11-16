#########################################################
##	onfire
##
FxInfo  


LifeSpan	300

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Local
		At	chest	
		Part1	:BaleFireTargetSmoke.part
		Part2	:BaleFireTargetFlames.part
		Part3	:BaleFireSparks.part
		sound	succubusclawhit 70 60 .77
		Lifespan	10
	End

	Event
		EName 	impact
		Type	Local 
		At	chest	
		Part	:HellBoltFlash.part
		Part1	:HellSparks.part
		sound	force1 70 60 .8
		Lifespan	3
				
	End


End


Condition
	On 	Time
	Time 	30

	Event
		EName 	All
		Type	Destroy 
	End


End


End			
