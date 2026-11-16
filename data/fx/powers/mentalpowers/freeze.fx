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

Input  
	Inpname	T_Chest
End
##########################

Condition
	On 	Time
	Time 	20

	Event
		EName	core
		Type	start
		At	head
		Bhvr	Behaviors/MentalScale.bhvr
		Geom	LocalPivot01	
		Sound mental6b 60 60 .71
		Magnet	T_Chest
		Lookat	T_Chest
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

	Event
		EName	lightning
		Type	start
		At	head
		part2	:FreezeSnow.part
		part4	:FreezeMist.part
		part5	:Freezesteam.part
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
	Time 	50

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
	Time	65

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