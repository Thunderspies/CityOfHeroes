#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End

Input  
	InpName	hips
End

##################################

Condition
	On	Time
	Time	0
		
	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part1	:IceBoltMistB.part
		Part2	:ChillTouchSnowA.part
		Sound icebuild2 80 80 .7
	End

End

Condition
	On	Time
	Time	30
		
	Event	
		Ename   IceShards
		Type	local
		At	hips
		
		Part3	:ChillingEmbraceRing.part
		#Sound	cold_loop 80 30 .8
	End

	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part1	:ColdStars.part
		Sound icearmor_loop 80 80 .7
		
	End
End

Condition
	On	Time
	Time	70
	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part2	:FrostBits.part
		
	End

	Event	
		Ename   IceShards
		Type	local
		At	hips
		Part2	:FrostBits.part
		
	End

End


End				