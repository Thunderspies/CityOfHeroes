#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	SpadR
End

Input  
	Inpname	SpadL
End

Input  
	Inpname	LarmL
End

Input  
	Inpname	LarmR
End

Flags    InheritAlpha

########################################################
## hookup
########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	link1
		Type	Local 
		At	SpadR

		Part1	:DieselSmoke.part
		Part2	:DieselSteam.part
		Sound diesel_loop 60 60 .55
		
	End

	Event
		EName 	link2
		Type	Local 
		At	SpadL

		Part1	:DieselSmoke.part
		Part2	:DieselSteam.part
		
	End

	Event
		EName 	link3
		Type	Local 
		At	LarmL

		Part3	AnimatedCharacterParts/SprocketNeckSpark2.part
		Part4	AnimatedCharacterParts/SprocketNeckGlow.part
		
	End

	Event
		EName 	link4
		Type	Local 
		At	LarmR

		Part3	AnimatedCharacterParts/SprocketNeckSpark2.part
		Part4	AnimatedCharacterParts/SprocketNeckGlow.part
		
	End
End

Condition
	On 	triggerbits
	Triggerbits	death

	Event
		EName 	link1
		Type	Destroy
	
	End

	Event
		EName 	link2
		Type	Destroy
	
	End

	Event
		EName 	link3
		Type	Destroy
	
	End

	Event
		EName 	link4
		Type	Destroy
	
	End

End

End
##################################

