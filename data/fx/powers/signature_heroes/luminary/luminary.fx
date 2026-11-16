#########################################################
##	template

FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Col_R
End

Input  
	Inpname	Col_L
End

Input  
	Inpname	Emblem
End

Input  
	Inpname	UArmR
End

Input  
	Inpname	UArmL
End

Input  
	Inpname	ULegR
End

Input  
	Inpname	ULegL
End

Input  
	Inpname	Hair
End

Input  
	Inpname	Chest
End

Input  
	Inpname	Head
End

#########################################################
###### HEAD GLOWS #####################################
#########################################################


Condition
	On 	Time  
	Time 	0
	
	Event
		EName 	Luminary_Head_Node
		Type	Local 
		At	Head
		GEOM	Hero_Eye_Node
	
	End

End

Condition
	On 	Time
	Time 	0
	
	Event
		EName 	LuminaryEyes
		Type	Local 
		At	Luminary_Head_Node
		AltPiv	1
		Part	:LuminaryGlow.part
		#Part	:LuminaryEyeRight.part

	End
	Event
		EName 	LuminaryEyes
		Type	Local 
		At	Luminary_Head_Node
		AltPiv	2
		Part	:LuminaryGlow.part
		#Part	:LuminaryEyeLeft.part

	End


End

#########################################################
###### KILL FX  #####################################
#########################################################

Condition
	On		Triggerbits
	Triggerbits	Death

	Event		
		Ename	All
		Type	Destroy

	End

End

End


