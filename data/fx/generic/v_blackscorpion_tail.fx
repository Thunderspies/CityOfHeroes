#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha   InheritAnimScale

Input  
	InpName	hips
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	waist
		Anim	V_Black_Scorpion_Tail ##ent type
		Bhvroverride
			PositionOffset	0 -.62 -.952
		end
	End
End

End	



			