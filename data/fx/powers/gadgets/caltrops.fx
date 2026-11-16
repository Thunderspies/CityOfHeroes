#########################################################
##	forceBubble
########################################################
FxInfo

Input  
	Inpname	Root
End

#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	lazer
		Type	start
		At	root
			
		Bhvr	Behaviors/caltropsFadein.bhvr	
		Geom	caltrops
		
		
	End

		
End


End