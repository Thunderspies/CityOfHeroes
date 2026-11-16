#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	200

Input  
	Inpname	chest
End

Input  
	Inpname	head
End

Input  
	Inpname	root
End
##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	chest
		part1	:DisruptionRings.part
		part2	:DisruptionRingz.part
		
		#part3	:ScreechGlow.part
		#part4	:ScreechRingsRooted.part
		#part5	:ScreechRingzRooted.part

	End

	
End

Condition
	On 	Time
	Time	26
	
	Event
		Ename 	Explosion
		Type	Local
		At	root
		Part1	:DisruptionExplosiveRing.part
		Part3	:DisruptionExplosiveRing3.part
		Part5	:DisruptionExplosiveRing5.part
		Part2	:DisruptionDamageRing.part
	End
End

Condition
	On 	Time
	Time	38
	
	Event
		Ename 	Explosion3
		Type	Local
		At	root
		Part1	:DisruptionExplosiveRing.part
		Part3	:DisruptionExplosiveRing3.part
		Part5	:DisruptionExplosiveRing5.part
		Part2	:DisruptionDamageRing.part
		
	End

End

Condition
	On 	Time
	Time	50
	
	Event
		Ename 	Explosion5
		Type	Local
		At	root
		Part1	:DisruptionExplosiveRing.part
		Part3	:DisruptionExplosiveRing3.part
		Part5	:DisruptionExplosiveRing5.part
		Part2	:DisruptionDamageRing.part
	End
End

Condition
	On 	Time
	Time	30
	
	Event
		Ename 	core
		Type	Destroy
	End


End





End