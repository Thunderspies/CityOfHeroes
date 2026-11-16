#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	100

Input  
	Inpname	head
End

######################
##Head effect
##########################
Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental9 80 80 .8
		
	End


	Event
		EName	core2
		Type	Local
		At	core
		  Altpiv   4
		
		part1	:MindControlRingsSmall.part
		part2	:MindControlRingzSmall.part
		
	End

End

Condition
	On 	Time
	Time 	10

Event
		EName	core3
		Type	Local
		At	core
		  Altpiv   4
		
		part2	:MindGlow2.part
				
	
	End

End

Condition
	On 	Time
	Time	90
	
	Event
		Ename 	core3
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	76
	
	Event
		Ename 	core2
		Type	Destroy
	End

End





End