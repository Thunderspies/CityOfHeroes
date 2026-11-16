#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	Orgin
End

Input  
	InpName	Hips
End

Input  
	InpName	Chest
End

Input  
	InpName	FootR
End

Input  
	InpName	FootL
End


##################################
Condition
	On	Time
	Time	0
		
	Event	
		Ename   CloudAura
		Type	local
		At	Hips
		part1	:DodgeCloud.part
		PMagnet  Hips
	End
		
	Event	
		Ename   CloudAura
		Type	local
		At	Hips
		part1	:DodgeCloudlocal.part
		part2	:DodgeCloud2local.part
		PMagnet  Orgin
	End

	Event	
		Ename   Streaks
		Type	posit
		At	Chest
		Part1	:SpeedStreaksLocal.part
		part2	:DodgeStreaks.part
		PMagnet  Orgin
	End

	Event	
		Ename   LeftFootSpots
		Type	Posit
		At	FootL
		part1	:DodgeSpots.part
		PMagnet  Hips
	End

	Event	
		Ename   RightFootSpots
		Type	Posit
		At	FootR
		part1	:DodgeSpots.part
		PMagnet  Hips
	End


End

End				