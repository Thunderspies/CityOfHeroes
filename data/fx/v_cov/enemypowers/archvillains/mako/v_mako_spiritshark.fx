#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha  # InheritAnimScale

Input  
	InpName	hips
End


Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	neck
		Anim	Shark_Spirit_Frenzy ##ent type
		Bhvroverride
			scale .3 .3 .3
			PyrRotate	0 190 30
			PositionOffset	0 -.62 .75
		end
		setstate SPIRITSHARKFRENZY
	End
End

End	



			