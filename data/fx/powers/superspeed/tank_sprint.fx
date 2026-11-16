#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Root
End


Input  
	InpName	T1_L
End

Input  
	InpName	T1_R
End

Input  
	InpName	F1_L
End

Input  
	InpName	F1_R
End

##################################
Condition
	On	Time
	Time	0
		
	Event
		EName 	Prime
		Type	Local 
		At	T1_R
		#part2	:SpeedGlowLocal02.part	
		part3	:SprintRocks.part
		Part4	:SpeedCloudBaseGlow2.part
		Part5	:SpeedCloudBaseGlow3.part
		
	End

	Event
		EName 	Prime2
		Type	Local 
		At	T1_L
		#part2	:SpeedGlowLocal02.part	
		part3	:SprintRocks.part
		Part4	:SpeedCloudBaseGlow2.part
		Part5	:SpeedCloudBaseGlow3.part
		
	End

	Event
		EName 	Prime3
		Type	Local 
		At	F1_L
		#part2	:SpeedGlowLocal02.part	
		part3	:SprintRocks.part
		Part4	:SpeedCloudBaseGlow2.part
		Part5	:SpeedCloudBaseGlow3.part
		
	End

	Event
		EName 	Prime4
		Type	Local 
		At	F1_R
		#part2	:SpeedGlowLocal02.part	
		part3	:SprintRocks.part
		Part4	:SpeedCloudBaseGlow2.part
		Part5	:SpeedCloudBaseGlow3.part
		
	End

	
End				