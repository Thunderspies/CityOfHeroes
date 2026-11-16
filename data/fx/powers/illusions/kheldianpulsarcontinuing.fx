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

Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound forcefield2_loop 80 80 .22
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	EyeNode
		Type	Local
		At	Hair
		Geom	HairToEyesAdjustment
	End

	Event	
		ENAME	ChestGlow
		Type	Local
		At	EyeNode
		AltPiv	1
		part	:IllusBlindGlow01.part
		part	:IllusBlindOrbits01.part
		POther Neck
		#Bhvr	Behaviors/TelportationFade.bhvr
		#Lifespan 30
	
	End


End	

End