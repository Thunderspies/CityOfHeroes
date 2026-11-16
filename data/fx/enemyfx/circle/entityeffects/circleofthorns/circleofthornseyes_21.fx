#########################################################
##	Fireweapon
########################################################
FxInfo

Input  
	Inpname	Eyes
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
		Part1	:CircleOfThornsEyes_21Green.part
		Part2	:CircleOfThornsEyes_21Yellow.part
		Part3	:CircleOfThornsEyesMistLeft.part
		#Sound	thorneyes5_loop 30 28 .5
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:CircleOfThornsEyes_21Green.part
		Part2	:CircleOfThornsEyes_21Yellow.part
		Part3	:CircleOfThornsEyesMistRight.part
	End

End

End
##################################

