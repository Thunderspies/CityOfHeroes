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
		Sound Mental1 80 80 .8
	End

	Event
		EName	InsideCircle
		Type	Local
		At	core
		  Altpiv  5
		
		Geom	InnerCircle
		bhvr	behaviors/Pinwheelspin.bhvr
		
	
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		  Altpiv  5
		
		Geom	OutsideCircle
		bhvr	behaviors/OppositePinWheelSpin.bhvr
		
	
	End

	Event
		EName	alt
		Type	Local
		At	core
		  Altpiv   7
		
		part3	:LargeExpandingMindControlRing.part
	End


End

Condition
	On 	Time
	Time 	20

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
	Time 	47

	Event
		
		EName	alt2
		Type	Local
		At	core
		  Altpiv   6
		
		part1	:MindControlRingsBurst2.part

	End

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
		Ename 	alt
		Type	Destroy
	
	End

End


Condition
	On 	Time
	Time	100
	
	Event
		Ename 	all
		Type	Destroy
	End
End


End