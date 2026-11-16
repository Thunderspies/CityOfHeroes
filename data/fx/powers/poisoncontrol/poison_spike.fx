#########################################################
##	
##

FxInfo


Input  
	InpName	Mystic
End

LifeSpan	100

##################################
Condition
	On 	Time
	Time 	1



	Event

		Type	Local
		At	Mystic
		##Bhvr	behaviors/poisonspike.bhvr
		##Geom	poison_spike
		Anim	FX_poisonspike

	
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