#############################################################
## IceHitContinuing.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0


	Event
		Ename   Ice
		Type	local
		At	chest
		Part1	:IceChunks.part
		Part2	:IceChunksLarge.part
		Part	:iceScatter.part
		LifeSpan 53
	End

	Event
		Ename   IceShards
		Type	local
		At	chest
		Part1	:IceBoltMistA.part
		Part2	:ChillTouchSnowA.part
		Part4	:ChillTouchSnow.part
		#Sound 	Ice7 100.0 30.0 1.0
	End
End

#############################################################

End