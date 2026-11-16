#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input  
	InpName	Target
End

Input  
	InpName	Hair
End

Input  
	InpName	Head
End




#########################################################
####################### AUDIO ###########################
#########################################################




#########################################################
#################### BODY GLOW 1 ########################
#########################################################

Condition
	On	Time
	Time	14

	Event	
		ENAME	Hairnode
		Type	Local
		At	Hair
		#AltPiv	1
		part	:StunRings01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		Lifespan 0
	
	End


End	


End