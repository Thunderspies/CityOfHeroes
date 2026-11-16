#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

##################################
Condition
	On 	Time
	Time 	5



	Event

		Type	Local
		Bhvr	behaviors/swoosh.bhvr 
		At	Mystic
		Geom	swoosh





		
	End

End

##################################


Condition
	On 	Time
	Time 	10

	Event
		Ename 	All
		Type	Destroy
	End
End

End				