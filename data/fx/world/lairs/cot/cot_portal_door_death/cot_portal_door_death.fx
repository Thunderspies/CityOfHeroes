#########################################################
##	template

FxInfo

#########################################################


Lifespan	100

Condition
	On 	Time
	Time 	0	
	
	Event
		EName 	Curly
		Type	Start
		At	Hips
		Bhvr	:CoT_Portal_Altpivot1.bhvr
	End

	Event
		EName 	Larry
		Type	Start
		At	Curly
		Part	:Portal_Burst_01.part
		Lifespan	25
	End
	
End

Condition
	On 	Time
	Time 	30	
	
	Event
		EName 	Shemp
		Type	Start
		At	Curly
		Part	:Portal_Burst_02.part
		#Lifespan	5
	End
	
End

Condition
	On 	Time
	Time 	35	
	
	Event
		EName 	Moe
		Type	Start
		At	Curly
		Part	:Portal_Burst_03.part
		#Lifespan	5
	End
	
End

End
