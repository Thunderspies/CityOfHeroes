#########################################################
##	
##
FxInfo

Input  
	InpName	Origin
End

##Lifespan 1

Condition
	On Time
	Time 0
	Event
		EName 	Bird_1
		Type	Local
		At	root
		Anim	FX_seagull_flock
#		Bhvr	behaviors/flock_fly.bhvr
	End

End

Condition
	On Time
	Time 55
	Event
		EName 	Bird_2
		Type	Local
		At	root
		Anim	FX_seagull_flock
		Bhvr	behaviors/flock_offset_1.bhvr
	End

End


Condition
	On Time
	Time 80
	Event
		EName 	Bird_3
		Type	Local
		At	root
		Anim	FX_seagull_flock
		Bhvr	behaviors/flock_offset_2.bhvr
	End

End


Condition
	On Time
	Time 120
	Event
		EName 	Bird_4
		Type	Local
		At	root
		Anim	FX_seagull_flock
		Bhvr	behaviors/flock_offset_3.bhvr
	End

End


Condition
	On Time
	Time 300
	Event
		EName 	Bird_5
		Type	Local
		At	root
		Anim	FX_seagull_flock
		Bhvr	behaviors/flock_offset_4.bhvr
	End

End


Condition
	On Time
	Time 400
	Event
		EName 	Bird_6
		Type	Local
		At	root
		Anim	FX_seagull_flock
		Bhvr	behaviors/flock_offset_5.bhvr
	End

End


End						