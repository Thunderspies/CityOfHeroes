#########################################################
##	
##
FxInfo

Lifespan  100



Condition
	On Time
	Time 0		##(player is invisible)
	Event
		EName 	Prime
		Type	Local
		At	Origin 
		Bhvr	behaviors/invisible.bhvr
		Flags	AdoptParentEntity
		Lifespan	15

	End
End


#Condition
#	On time
#	Time 31
#	
#	Event
#		EName 	Prime2	##(player fades in)
#		Type	Local
#		At	Origin 
#		Bhvr	behaviors/fadein.bhvr
#		Flags	AdoptParentEntity
#
#	End
#End


End			