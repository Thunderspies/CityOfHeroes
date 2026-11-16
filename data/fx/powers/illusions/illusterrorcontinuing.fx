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
Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound terror2 80 80 .7
	End
End


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound terror2_loop 80 80 .4
	End
End




#########################################################
#################### BODY GLOW 1 ########################
#########################################################



Condition
	On	Time
	Time	30
	
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
		At	Eyes
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

############################## SUMMON ##############################
Condition
	On	Time
	Time	0
	
	Event	
		ENAME	ChestMagnet
		Type	Start
		At	Chest
		#Geom	Junk_Car_01
		part	:IllusTerrorSummonCloudIn.part
		part	:IllusTerrorSummonGlows.part
		Lifespan 35
	
	End

End

Condition
	On	Time
	Time	35
	
	Event	
		ENAME	ChestMagnet
		Type	Start
		At	Head
		#Geom	Junk_Car_01
		part	:IllusTerrorSummonGlowStatic.part
		part	:IllusTerrorSummonRays01.part
		Lifespan 15
	
	End

End

End
##################################

