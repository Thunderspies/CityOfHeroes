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
		EName 	Bugs
		Type	start 
		At	root
		Anim	Spider_Swarm_ent
		Bhvroverride
			scale	.2 .2 .2
		end
	End

	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET
		AnimPiv T1_R
		Anim	V_Spider_Swarmling
		Bhvroverride
			scale	.9 .9 .9
		end
	End
	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET

		AnimPiv T2_R
		Anim	V_Spider_Swarmling
	End
	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET

		AnimPiv T3_R
		Anim	V_Spider_Swarmling
		Bhvroverride
			scale	.9 .9 .9
		end
	End
	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET

		AnimPiv T1_L
		Anim	V_Spider_Swarmling
	End
	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET

		AnimPiv T2_L
		Anim	V_Spider_Swarmling
		Bhvroverride
			scale	.9 .9 .9
		end
	End
	
	Event
		Type	Local 
		At	Bugs 
		SetState	FEET

		AnimPiv T3_L
		Anim	V_Spider_Swarmling
	End
End

End	



			