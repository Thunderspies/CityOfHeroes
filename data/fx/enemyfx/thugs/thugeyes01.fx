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
		Geom	FX_Thug_Eyes
		Sound thorneyes2_loop 15 15 .18
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	:ThugEyesGreen.part
		Part2	:ThugEyesYellow.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:ThugEyesGreen.part
		Part2	:ThugEyesYellow.part
		
	End

End

End
##################################

