#########################################################
##	template

FxInfo

#########################################################


Lifespan	100

Condition
	On 	Time
	Time 	00
	Event
		EName 	DoorCenter
		Type	local 
		At	Hips
		Bhvr	:CoT_Door_Altpivot.bhvr
	End
End

Condition
	On 	Time
	Time 	00
	Event
		EName 	DoorCenter2
		Type	local 
		At	Hips
		Bhvr	:CoT_Door_Altpivot2.bhvr
	End
End

Condition
	On 	Time
	Time 	00
	Event
		EName 	DoorCenter3
		Type	local 
		At	Hips
		Bhvr	:CoT_Door_Altpivot3.bhvr
	End
End


Condition
	On 	Time
	Time 	10	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	local
		At	Hips
		Part	:Door_Pop_01.part
		Part	:Door_Pop_02.part
		Lifespan	5
	End	
End

Condition
	On 	Time
	Time 	15	
	
	Event
		EName 	Hydra_Head_Explosion
		Type	local
		At	DoorCenter3
		Part	:Door_Pop_02.part
		#Lifespan	30
	End	
End

