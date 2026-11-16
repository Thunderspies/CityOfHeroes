#########################################################
##	template

FxInfo

#########################################################


##Lifespan	500

Condition
	On 	Time
	Time 	0	

	Event
		EName 	Start_Point_Right
		Type	local
		At	Hips
		Bhvr	:Offset_Start_Point_Right.bhvr
	End	

	Event
		EName 	Start_Point_Left
		Type	local
		At	Hips
		Bhvr	:Offset_Start_Point_Left.bhvr
	End	

	Event
		EName 	End_Point
		Type	local
		At	Hips
		Bhvr	:Offset_End_Point.bhvr
	End	
	
	Event
		EName 	Mist
		Type	local
		At	Start_Point_Right
		Part	:Mist_Right_01.part
		PMagnet	End_Point
	End	

	Event
		EName 	Mist
		Type	local
		At	Start_Point_Left
		Part	:Mist_Left_01.part
		PMagnet	End_Point
	End	
End

Condition
	On 	Time
	Time 	10	

	Event
		EName 	Mist
		Type	local
		At	Start_Point_Right
		Part	:Mist_Right_02.part
		PMagnet	End_Point
	End	

	Event
		EName 	Mist
		Type	local
		At	Start_Point_Left
		Part	:Mist_Left_02.part
		PMagnet	End_Point
	End	
End

Condition
	On 	Time
	Time 	0	

	Event
		EName 	Mist
		Type	local
		At	Hips
		Part	:Mist_Stack.part
		Bhvr	:Offset_Smokestack.bhvr
	End	
End

