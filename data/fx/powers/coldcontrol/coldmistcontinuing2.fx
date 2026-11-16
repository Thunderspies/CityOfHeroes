#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	chest
End

##################################

Condition
	On	Time
	Time	0
		

	Event	
		Ename   Ice
		Type	local
		At	chest
		#Part1	:IceChunks.part
		#Part2	:IceChunksLarge.part
		#Part3	:IceScatter.part
		#Part4	:ChillTouchSnow.part
		
		
	End

	Event	
		Ename   IceShards
		Type	local
		At	chest
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		
	End


End


End				