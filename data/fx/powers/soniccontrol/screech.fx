#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	50

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		#bhvr	behaviors/PsionicsScale.bhvr
	End

	Event
		EName	core3
		Type	Local
		At	core
		  Altpiv   4
		part1	:DisruptionSpark.part
		part2	:ScreechGlow.part
		part3	:ScreechRingsRooted.part
		part4	:ScreechRingzRooted.part
		part5	:DisruptionSparkStreaks.part
		
	
	End

End

Condition
	On 	Time
	Time 	5
	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   4
		
		part1	:ScreechRings.part
		part2	:ScreechRingz.part
		
	End

End

Condition
	On 	Time
	Time	45
	
	Event
		Ename 	core3
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	50
	
	Event
		Ename 	all
		Type	Destroy
	End
End





End