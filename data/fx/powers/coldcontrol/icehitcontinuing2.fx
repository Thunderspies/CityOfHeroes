#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End

##################################

Condition
	On	Time
	Time	30
		

	Event	
		Ename   Ice
		Type	local
		At	chest
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		Part	:iceScatter.part		
		LifeSpan	53
		
	End

	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		Part4	:ChillTouchSnow.part
		Sound Ice7 100.0 100.0 1.0
	End


End


End				