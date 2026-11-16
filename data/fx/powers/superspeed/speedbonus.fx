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
		#part2	:SpeedGlowLocal02.part	
		#part3	:SpeedEmberLocal.part
		#part4	:SpeedEmber.part
		#Part5	:SpeedCloudBaseGlow.part
		Geom	Dummy02	
		Sound Run5 100.0 100.0 .8
	End

	Event
		Type	Local 
		At	Root
		#part1	:SpeedCloudBaseGlowFlat.part
		#part2	:SpeedStreaks.part
		#part3	:SpeedStreaks.part
		Sound ForceField5_loop 100.0 100.0 .77
	End


	Event
		
		Type	Local 
		At	Prime
		AltPiv  1
		#part1	:SpeedFlame01.part
		#part2	:SpeedFlame01Flat.part
		#part5	:SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		AltPiv	2
		#part1	:SpeedFlame01.part
		#part2	:SpeedFlame01Flat.part
		#part5	:SpeedFire.part
	End

	Event
		
		Type	Local 
		At	Prime
		AltPiv  1
		#Part2	:SpeedCloudStreaks.part
		#part4	:SpeedFire.part
		#part5	:SpeedGlow.part
		
	End

	Event
		
		Type	Local 
		At	Prime
		AltPiv  2
		#Part2	:SpeedCloudStreaks.part
		#part4	:SpeedFire.part
		#part5	:SpeedGlow.part
	End


	Event	
		
		Type	Local
		At	FootR
		#part1	:SpeedSpots.part
		PMagnet Head
	End

	Event	
		Type	Local
		At	FootL
		#part1	:SpeedSpots.part
		PMagnet Head
	End

	Event
		Type	Local
		At	Hips
		#part1	:SpeedCloud.part
		
	End

	Event
		Type	Local
		At	Hips
		#part2	:SpeedCloudLocal.part
		PMagnet   Hips
	End
End

End				