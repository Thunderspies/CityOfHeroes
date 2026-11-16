#########################################################
##	template

FxInfo

Input  
	Inpname	Root
End


#########################################################

Condition
	On 	Time
	Time 	0

	Event 
		Type	start 
		At	Root 
		Bhvr	:Lifter2.bhvr
		Geom	teleport
		#Part	:lifterclouds.part
		Part	:liftertrail.part
		Part	:lifterbase.part
		Part	:lifterrings.part
		Part	:liftersparkles.part
	End
End


End		