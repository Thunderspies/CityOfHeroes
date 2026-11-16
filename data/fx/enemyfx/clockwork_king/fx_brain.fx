#########################################################
##	jack
##
FxInfo

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime
		Type	Local 
		At	Eyes
		Anim	FX_brain
		Sound clockworkking2_loop 64 64 .68
	End

	Event
		EName 	Bubbles
		Type	Local 
		At	Neck
		Part1	:BrainBubbles.part
	End

End

End			