#########################################################
##	TemperatureControl
########################################################

FxInfo

Input  
	Inpname	root
End

Input  
	Inpname	head
End
##########################
Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01	
		Sound mental7c 80 80 .8
	End

	Event
		EName	InsideCircle
		Type	Local
		At	core
		  Altpiv  5
		
		Geom	InnerCircle
		bhvr	behaviors/2Pinwheelspin.bhvr
		
	
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		  Altpiv  5
		
		Geom	OutsideCircle
		bhvr	behaviors/2OppositePinWheelSpin.bhvr
		
	
	End

	Event
		EName	alt
		Type	Local
		At	core
		  Altpiv   7
		
			#part1	:LargeMindControlRing.part
		
		part3	:LargeExpandingMindControlRing.part
	End

End

Condition
	On 	Time
	Time 	30

	Event
		EName	lightning
		Type	Local
		At	head
		part2	:TerrifyElectricity.part
		part4	:TerrifyElectricity2.part
	End

End

Condition
	On 	Time
	Time 	10

Event
		EName	alt1
		Type	Local
		At	core
		  Altpiv   6
		
		part1	:MindControlRingsBurst2.part

	End

End

Condition
	On 	Time
	Time 	25

Event
		EName	alt2
		Type	Local
		At	core
		  Altpiv   6
		
		part1	:MindControlRingsBurst2.part

	End

End

Condition
	On 	Time
	Time 	40

Event
		EName	alt3
		Type	Local
		At	core
		  Altpiv   6
		
		part1	:MindControlRingsBurst2.part

	End

End


Condition
	On 	Time
	Time	50
	
	Event
		Ename 	InsideCircle
		Type	Destroy
	End

	Event
		Ename 	OutsideCircle
		Type	Destroy
	End

End

Condition
	On 	Time
	Time	70

	Event
		Ename 	lightning
		Type	Destroy
	
	End

End

Condition
	On 	Time
	Time	60

	Event
		Ename 	alt
		Type	Destroy
	
	End

End


Condition
	On 	Time
	Time	80
	
	Event
		Ename 	all
		Type	Destroy
	End
End


End