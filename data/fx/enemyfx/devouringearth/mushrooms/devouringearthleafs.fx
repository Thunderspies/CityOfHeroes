#########################################################
##	
##
FxInfo

Input  
	InpName	CHEST
End

Input  
	InpName	UARMR
End

Input  
	InpName	UARML
End

Input  
	InpName	LARML
End

Input  
	InpName	LARMR
End

Input  
	InpName	LLEGL
End

Input  
	InpName	LLEGR
End

Input  
	InpName	Head
End

Flags    InheritAlpha

#####################################################################################
##Lens Flares
#####################################################


#Condition
#	On	Time
#	Time 	0
#		
#	Event
#		Type	Local
#		At 	origin
#		Sound	Stone4 100 30 .96
#	End
#End
#
#Condition
#	On	Time
#	Time 	8
#		
#	Event
#		Type	Local
#		At 	origin
#		Sound	Mineral_loop 100 30 .72
#	End
#End
#
#Condition
#	On	Time
#	Time 	8
#		
#	Event
#		Type	Local
#		At 	origin
#		Sound	glow6_loop 100 30 .55
#	End
#End
#
#

Condition
	On 	cycle
	Time 	20
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	Chest
		part1	:PlantGuyLeaves.part
		
		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	20
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	LarmL
		part1	:PlantGuyLeaves.part
		

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	19
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	LarmR
		part1	:PlantGuyLeaves.part
		

		lifespan	25
	End
	
End

Condition
	On 	cycle
	Time 	21
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	UarmL
		part1	:PlantGuyLeaves.part
		

		lifespan	25
	End
	
End


Condition
	On 	cycle
	Time 	22
	Chance	.12
	
	Event
		EName	3
		Type	Local
		At	UarmR
		part1	:PlantGuyLeaves.part
		

		lifespan	25
	End
	

End

Condition
	On 	cycle
	Time 	17
	Chance	.12

	Event
		EName	3
		Type	Local
		At	Head
		part1	:PlantGuyLeaves.part
		
		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	17
	Chance	.12

	Event
		EName	3
		Type	Local
		At	LlegL
		part1	:PlantGuyLeaves.part
		

		lifespan	25
	End

End

Condition
	On 	cycle
	Time 	18
	Chance	.12

	Event
		EName	3
		Type	Local
		At	LlegR
		part1	:PlantGuyLeaves.part
		
		lifespan	25
	End


End

#####################################################################################


End	