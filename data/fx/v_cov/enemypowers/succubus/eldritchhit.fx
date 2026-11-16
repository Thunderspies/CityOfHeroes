#########################################################
##	onfire
##
FxInfo  


LifeSpan	200

Condition
	On 	Time
	Time 	0

	Event
		EName 	Fire
		Type	Local
		At	chest		
		Part	:HellBoltFlash.part
		Part1	:BaleFireTargetSmoke.part
		Part2	:BaleFireTargetFlames.part
		Part3	:BaleFireSparks.part	
		Sound FireExplo 100.0 100.0 .46
	End


End


Condition
	On 	Time
	Time 	120

	Event
		EName 	All
		Type	Destroy 
	End


End


End			
