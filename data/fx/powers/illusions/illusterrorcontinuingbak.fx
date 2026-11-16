#########################################################
##	template

FxInfo

Input
	InpName Origin
End

Input
	InpName Target
End

Input  
	InpName	Root
End

Input  
	InpName	Hair
End

Input  
	InpName	Head
End

Input  
	InpName	Eyes
End

Input  
	InpName	Neck
End

Input  
	InpName	Chest
End

Input  
	InpName	SpadR
End

Input  
	InpName	SpadL
End

Input  
	InpName	UarmR
End

Input  
	InpName	UarmL
End

Input  
	InpName	LarmR
End

Input  
	InpName	LarmL
End

Input  
	InpName	HandR
End

Input  
	InpName	HandL
End

Input  
	InpName	Hips
End

Input  
	InpName	T1_L
End

Input  
	InpName	T1_R
End

Input  
	InpName	T2_L
End

Input  
	InpName	T2_R
End

Input  
	InpName	F1_L
End

Input  
	InpName	F1_R
End

Input  
	InpName	RingL
End

Input  
	InpName	RingR
End

Input  
	InpName	F2_L
End

Input  
	InpName	F2_R
End



#########################################################
####################### AUDIO ###########################
#########################################################



########################################################



#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	0
	
	Event	
		ENAME	ChestGlowMain
		Type	Local
		At	Neck
		part	:IllusTerrorChestGlow01.part
		#part	:IllusTerrorRays01.part
		part	:IllusTerrorRisingGlows01.part
		POther  Hips
	
	End

	Event	
		ENAME	RightChestGlow
		Type	Local
		At	UarmR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorBodyGlow01.part
		part	:IllusTerrorRays01.part
		POther  Hips
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	LeftChestGlow
		Type	Local
		At	UarmL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorBodyGlow01.part
		part	:IllusTerrorRays01.part
		POther  Hips
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End
	
	Event	
		ENAME	Face
		Type	Local
		At	Hair
		part	:IllusTerrorFace01.part
		part	:IllusTerrorRisingGlows01.part
		POther  Chest
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	RightUarmGlow
		Type	Local
		At	SpadR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr	
		
	End

	Event	
		ENAME	LeftUarmGlow
		Type	Local
		At	SpadL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	Event	
		ENAME	RightLarmGlow
		Type	Local
		At	HandR
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmR
		#Bhvr	Behaviors/TelportationFade.bhvr
		
		
	End

	
	Event	
		ENAME	LeftLarmGlow
		Type	Local
		At	HandL
		part	:IllusTerrorGlow01.part
		part	:IllusTerrorRisingGlows01.part
		part	:IllusTerrorBodyGlow01.part
		POther  LarmL
		#Bhvr	Behaviors/TelportationFade.bhvr
	
	End

	Event	
		ENAME	LeftHandGlow
		Type	Local
		At	HandL
		part	:IllusTerrorWristGlow01.part
		POther  RingL
		#Bhvr	Behaviors/TelportationFade.bhvr

	End

	Event	
		ENAME	RightHandGlow
		Type	Local
		At	HandR
		part	:IllusTerrorWristGlow01.part
		POther  RingR
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	RightFing1Glow
		Type	Local
		At	RingR
		part	:IllusTerrorHandGlow01.part
		POther  F2_R
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	LeftFing1Glow
		Type	Local
		At	RingL
		part	:IllusTerrorHandGlow01.part
		POther  F2_L
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	LeftFing1Glow
		Type	Local
		At	T1_L
		part	:IllusTerrorHandGlow01.part
		POther  F1_L
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	RightFing1Glow
		Type	Local
		At	T1_R
		part	:IllusTerrorHandGlow01.part
		POther  F1_R
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	LeftThumbGlow
		Type	Local
		At	HandL
		part	:IllusTerrorHandGlow01.part
		POther  T2_L
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

	Event	
		ENAME	RightThumbGlow
		Type	Local
		At	HandR
		part	:IllusTerrorHandGlow01.part
		POther  T2_R
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End
############################## OLD HANDS ##############################
	Event	
		ENAME	HandLeft
		Type	Local
		At	HandL
		part	:IllusTerrorHand02.part
		#Bhvr	Behaviors/TelportationFade.bhvr

		
	End

	Event	
		ENAME	HandRight
		Type	Local
		At	HandR
		part	:IllusTerrorHand02.part
		#Bhvr	Behaviors/TelportationFade.bhvr
		
	End

End

End
##################################

