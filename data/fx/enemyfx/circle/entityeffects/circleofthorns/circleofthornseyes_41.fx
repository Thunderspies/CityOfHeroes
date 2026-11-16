#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
End

Input  
	Inpname	Head
End

Flags    InheritAlpha


########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Eyes
		Geom	CircleOfThorns
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	:CircleOfThornsEyes_41Green.part
		Part2	:CircleOfThornsEyes_41Yellow.part
		#Part3	:CircleOfThornsEyesMistLeft.part
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:CircleOfThornsEyes_41Green.part
		Part2	:CircleOfThornsEyes_41Yellow.part
		#Part3	:CircleOfThornsEyesMistRight.part
	End

	Event
		ENAME	Headglow	
		Type	Local 
		At	Head
		Part1	:CircleOfThornsDieity.part
		Part2	:CircleOfThornsDieitySmall.part
		
	End

End

Condition
	On 	TriggerBits
	TriggerBits	Death

	Event
		EName 	Headglow
		Type	Destroy
	
	End
	
End

End
##################################
