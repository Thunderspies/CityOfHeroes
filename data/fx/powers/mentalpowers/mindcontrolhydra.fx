#########################################################
##	TemperatureControl
########################################################

FxInfo

Lifespan 105

##########################
Condition
	On 	Time
	Time 	13


	Event
		EName	looker
		Type	Posit
		At	chest
		LookAt	Target	
	End

	Event
		EName	core
		Type	Local
		At	looker
		Bhvr	POWERS/mentalpowers/HydraMindControl.bhvr
		Sound mental4 80 80 .75
	End

	Event
		EName	InsideCircle
		Type	Local
		At	core
		
		Geom	InnerCircle
		bhvr	behaviors/Pinwheelspin.bhvr
		
	
	End

	Event
		EName	OutsideCircle
		Type	Local
		At	core
		
		Geom	OutsideCircle
		bhvr	behaviors/OppositePinWheelSpin.bhvr
		
	
	End

 ##############These two bhvrs are just because the particles are set up totally crazy, and I don't want to go in and fix them
	Event
		EName	alt
		Type	Local
		At	core
		Bhvr	POWERS/mentalpowers/HydraMindControl4.bhvr
		part	:ExpandingMindControlRing.part

	End

	Event
		EName	alt2
		Type	Local
		At	core
		Bhvr	POWERS/mentalpowers/HydraMindControl4.bhvr 
		part	:LargeMindControlRing.part
		
	End
End

 
Condition
	On 	Time
	Time	40
	
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
	Time	90

	Event
		Ename 	alt2
		Type	Destroy
	
	End

End

Condition
	On 	Time
	Time	57

	Event
		Ename 	alt
		Type	Destroy
	
	End

End


End