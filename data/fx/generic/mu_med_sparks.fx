#########################################################
##	FireRing
##
FxInfo

#LifeSpan	70	

Input  
	InpName	Origin
End

Input  
	InpName	Target
End

Input  
	InpName	root
End

Input  
	Inpname	head
End

Condition
	On 	Cycle
	Time 	11
	
#	On 	Time
#	Time 	0

	Event
		EName 	ElecSpinner
		Type	Local 
		At	root
		bhvr	behaviors/mu_Spin_elec.bhvr
		Lifespan 5
	End

End

Condition
	On 	Cycle
	Time 	5


	Event
		EName 	ElecSpinnerOffset
		Type	posit
		At	ElecSpinner
		bhvr	behaviors/mu_SpinOff_elec.bhvr
		Lifespan 5
				
	End

End

Condition
	On 	Cycle
	Time 	5

	Event
		EName	lightning
		Type	Local  
		At	ElecSpinnerOffset
		Part5	:mu_sparks.part
		Lifespan 5
	End

End	


Condition
	On 	Cycle
	Time 	18
	
#	On 	Time
#	Time 	0

	Event
		EName 	ElecSpinner1
		Type	Local 
		At	root
		bhvr	behaviors/mu_Spin_elec1.bhvr
		Lifespan 6
	End

End

Condition
	On 	Cycle
	Time 	5


	Event
		EName 	ElecSpinnerOffset1
		Type	posit
		At	ElecSpinner1
		bhvr	behaviors/mu_SpinOff_elec1.bhvr
		Lifespan 5
				
	End

End

Condition
	On 	Cycle
	Time 	5

	Event
		EName	lightning1
		Type	Local  
		At	ElecSpinnerOffset1
		Part5	:mu_sparks1.part
		Lifespan 5
	End

End