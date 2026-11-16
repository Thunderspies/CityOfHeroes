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

Input  
	Inpname	Origin
End


Flags    InheritAlpha

########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		Sound SoulDrain5_loop 36 36 .58
	
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Eyes
		Geom	FX_Requiem
	
	End

	Event
		Type	Local 
		At	Prime
		 AltPiv 1
		Part1	:RequiemEyes.part
		Part2	:RequiemPupil.part
		
	End
	
	Event
		Type	Local 
		At	Prime
		 AltPiv 2
		Part1	:RequiemEyes.part
		Part2	:RequiemPupil.part
		
	End

	Event
		Type	Local
		At	Head
		
		Part1	:RequiemMist.part
		Part2	:RequiemMistAddative.part
	End
	
	Event
		Type	Local
		At	Head
		
		Part1	:RequiemMistTrail.part
		Part2	:RequiemMistFollow.part
	End

End

End
##################################

