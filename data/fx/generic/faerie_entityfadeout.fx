#########################################################
##	pump_smoke
##
FxInfo



Condition
	On Time
	Time 0
	
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/faeriefadeout.bhvr
		Flags	AdoptParentEntity
		Lifespan  5	##(This is when the fadeout behavior will start)
	End
End

Condition
	On Time
	Time 90		##(make this happen before the first contidition dies)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
	End
End

End			