#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha   InheritAnimScale

Input  
	InpName	hips
End



Condition

	On	Time
	Time	0

	Event
		Type	Local
		At	Waist
		Sound scorpionflame_loop 70 70 .26
	End
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	waist
		Anim	V_Signature_Dr_Quatrexin_Tail ##ent type
		Bhvroverride
			PositionOffset	0 -.62 -.952
			scale	0.75 0.75 0.75
		end
	End
End

End	



			