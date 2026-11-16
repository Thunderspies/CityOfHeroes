#########################################################
##	
########################################################
FxInfo

Input  
	Inpname	Root
End

Input  
	Inpname	Mystic
End

Input  
	InpName	Origin
End


##################################################################


##LifeSpan 900

Condition
	On	Time
	Time	0
	Event
		EName 	Prop
		Type	Start
		At	Root
		Geom	Prop_BoomBox_ground
		Bhvr	Behaviors/GenericParticleFade.bhvr

	End

End


Condition
On Time
Time 16
	Event
		Type	Local 
		At	Prop
		Sound Track10_loop 40.0 40.0 .9

	End

End




End


