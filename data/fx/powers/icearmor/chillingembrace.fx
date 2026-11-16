#############################################################
## Ice Armor - Chilling Embrace
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0
		
	Event	
		Ename   IceShards
		Type	local
		At	Chest
		Part1	:IceBoltMistB.part
		Part1	:ColdStars.part
		Part2	:ChillTouchSnowA.part
		Sound icebuild2 80 80 .7
	End
End

Condition
	On 	Cycle
	Time 	45
		
	Event	
		Ename   IceShards
		Type	Local
		At	Hips
		Part3	:ChillingEmbrace_Ellipse.part
		LifeSpan	45
	End
End

Condition
	On	Time
	Time	30

	Event	
		Ename   IceShards
		Type	local
		At	Chest
		bhvr	behaviors/soundFade3.bhvr
		Sound icearmor_loop 80 80 .7
		Flags	PowerLoopingSound
		Lifespan	250
	End
End

#############################################################

End				