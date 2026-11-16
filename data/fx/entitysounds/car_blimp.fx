#########################################################
##	Michael Henry
##
FxInfo

Input  
	InpName	Origin
End

Input  
	InpName	chest
End

#########################################################

Condition
	On Time
	Time 0

	Event
		Type	Local 
		At	Origin
		Sound Blimp_Close_Loop 200 150 1
	End

	Event
		Type	Local 
		At	Origin
		Sound Blimp_Distant_Loop 500 500 .4
	End

	Event
		Ename	Blimp
		Type	Local 
		At	chest
		Geom	blimphookup
	End

	Event
		Type	Local 
		At	Blimp
		Altpiv	1
		ChildFx	world/city/redlight02.fx
		bhvr	Behaviors/GenericParticleFade.bhvr
	End
	
	Event
		Type	Local 
		At	Blimp
		Altpiv	2
		ChildFx	world/city/redlight02.fx
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		Type	Local 
		At	Blimp
		Altpiv	4
		ChildFx	world/city/redlight02.fx
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event
		Type	Local 
		At	Blimp
		Altpiv	3
		ChildFx	world/city/redlight02.fx
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

End

End	



	