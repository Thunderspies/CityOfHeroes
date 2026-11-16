#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End

Input  
	InpName	Root
End

Input  
	InpName	Head
End

Input  
	InpName	Chest
End

Input  
	InpName	Hips
End

##################################
Condition
	On	Time
	Time	0
		
	Event
		EName 	Prime
		Type	Local 
		At	Root
		part2	:SprintGlowLocal02.part	
		part3	:SprintEmberLocal.part
		part4	:SprintEmber.part
		Part5	:SprintCloudBaseGlow.part
		Geom	Dummy02	
	End

	Event
		Type	Local 
		At	Root
		part1	:SprintCloudBaseGlowFlat.part
	End


	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
		part1	:SprintFlame01.part
		part2	:SprintFlame01Flat.part
		part5	:SprintFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
		part1	:SprintFlame01.part
		part2	:SprintFlame01Flat.part
		part5	:SprintFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 1
		Part2	:SprintCloudStreaks.part
		part4	:SprintFire.part
		part5	:SprintGlow.part
		
	End

	Event
		
		Type	Local 
		At	Prime
		 AltPiv 2
		Part2	:SprintCloudStreaks.part
		part4	:SprintFire.part
		part5	:SprintGlow.part
	End


End

End				