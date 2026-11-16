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
		Bhvr	:Malaise_Minions_Altpivot1.bhvr
		#Sound	thorneyes5_loop 30 28 .5
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:CircleOfThornsEyes_21Green.part
		Part2	:CircleOfThornsEyes_21Yellow.part
		Part3	:CircleOfThornsEyesMistRight.part
		Bhvr	:Malaise_Minions_Altpivot2.bhvr
	End
		Event
		Type	Local 
		At	Head
		Part1	:Head_Glow.part
		Bhvr	:Mother_Minions_Altpivot1.bhvr
	End

End

End
##################################

