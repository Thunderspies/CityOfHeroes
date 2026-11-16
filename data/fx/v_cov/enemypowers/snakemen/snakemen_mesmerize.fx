#########################################################
##	TemperatureControl
########################################################

FxInfo

LifeSpan	150

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################

Condition
	On	Time
	Time	0

	Event	
		EName	Emitter
		Type	Local
		At	Head

		bhvr	V_COV\EnemyPowers\SnakeMen\SM_B_MesmerizeEmit.bhvr
#		Geom	Testing_Target
	End
End

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Sound mental3 80 80 .8
	End


	Event
		EName	core2
		Type	Posit
		At	Emitter
		
		part1	V_COV\EnemyPowers\SnakeMen\SM_P_MesmerizeRings.part
		part2	V_COV\EnemyPowers\SnakeMen\SM_P_MesmerizeRings01.part
		
	End

End

Condition
	On 	Time
	Time 	30

Event
		EName	core3
		Type	Posit
		At	Emitter
		
		part2	POWERS/MentalPowers/MindGlow.part
				
	
	End

End

Condition
	On 	Time
	Time	125
	
	Event
		Ename 	core2
		Type	Destroy
	End
End


Condition
	On 	Time
	Time	165
	
	Event
		Ename 	all
		Type	Destroy
	End
End





End