#########################################################
##	jack
##
FxInfo
	
Flags  InheritAlpha   ##InheritAnimScale

Input  
	InpName	hips
End


Condition
	On 	Time
	Time 	0

	Event
		ename	workit
		Type	Local 
		At	larmr
		Anim	V_BlackScorpion_Hand ##ent type
	End
#	Event
#		Type	Local 
#		At	workit
#		AnimPiv T1_R
#		Anim	V_Spider_Swarmling	
#		Bhvroverride
#			scale	.01 .01 .01
#		end
#	End
End

End	



		