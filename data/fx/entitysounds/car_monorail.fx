#########################################################
##	Michael Henry
##
FxInfo


Input  
	InpName	Origin
End
Condition
	Event
		Type	Local 
		At	Origin
		Sound Train_Close_Loop 180 180 .7
	End
End
Condition
	On Cycle
	Time 300
	Chance 0.4
	Event
		Type	Posit 
		At	Origin
		SoundNoRepeat 1
		Sound Train_Bell_01 150 150 1
		Sound Train_Bell_02 150 150 1
		Sound Train_Bell_03 150 150 1
		lifespan 150
	End

	
End

Condition
	Event
		Type	Local 
		At	Origin
		Sound Train_Distant_Loop 500 500 .5
	End
End

End	



	