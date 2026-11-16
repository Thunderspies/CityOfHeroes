#########################################################
##	
##

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
		Sound coldBlast2 80 80 .7
		
	End

End

Condition
	On	Time
	Time	30
		
	Event	
		Ename   Icehookup
		Type	local
		At	chest
		Part1	:ColdStars.part
		#Part	:IceChunks.part
		Part	:IceChunksLarge2.part
	End

End

Condition
	On 	cycle
	Time 	35
	Chance	1
		
	Event	
		Ename   IceShards
		Type	local
		At	hips
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part3	:ChillingEmbraceRing.part
		LifeSpan	40
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


Condition
	On 	Time
	Time 	0

	Event
		EName 	spikes1
		Type	local
		At	chest
		geom	EarthDummy1	
		Bhvr	behaviors/StalagtiteCore.bhvr
		Sound icicle2_loop 80 80 .85
		
	End

End

Condition
	On 	cycle
	Time 	8
	Chance	.13


	Event
		EName 	Stonesickle01
		Type	local
		At	spikes1
		 AltPiv 1
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle01
		LifeSpan	10
	End

End

Condition
	On 	cycle
	Time 	6
	Chance	.18

	Event
		EName 	Stonesickle02
		Type	local
		At	spikes1
		 AltPiv 2
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle02
		LifeSpan	10
		
	End

End

Condition
	On 	Cycle
	Time 	7
	Chance	.17

	Event
		EName 	Stonesickle03
		Type	local
		At	spikes1
		 AltPiv 3
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle03
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	5
	Chance	.17

	Event
		EName 	Stonesickle04
		Type	local
		At	spikes1
		 AltPiv 4
		Bhvr	behaviors/Stalagtite2.bhvr
		geom	Icicle04
		LifeSpan	10	
	End

End

Condition
	On 	Cycle
	Time	9
	Chance	.18

	Event
		EName 	Stonesickle10
		Type	local
		At	spikes1
		 AltPiv 4
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle10
		LifeSpan	10
	End
End

Condition
	On 	cycle
	Time 	8
	Chance	.13

	Event
		EName 	Stonesickle05
		Type	local
		At	spikes1
		 AltPiv 5
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle05
		LifeSpan	10	
	End

End

Condition
	On 	cycle
	Time 	6
	Chance	.16

	Event
		EName 	Stonesickle06
		Type	local
		At	spikes1
		 AltPiv 6
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle06
		LifeSpan	10
	End

End

Condition
	On 	Cycle
	Time 	8
	Chance	.15

	Event
		EName 	Stonesickle07
		Type	local
		At	spikes1
		 AltPiv 6
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle07
		LifeSpan	10
	End

End


Condition
	On 	cycle
	Time 	5
	Chance	.17

	Event
		EName 	Stonesickle08
		Type	local
		At	spikes1
		 AltPiv 2
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle08
		LifeSpan	10
	End

End


Condition
	On 	cycle
	Time 	7
	chance	.14

	Event
		EName 	Stonesickle09
		Type	local
		At	spikes1
		 AltPiv 4
		Bhvr	behaviors/Stalagtite1.bhvr
		geom	Icicle09
		LifeSpan	10
	End

End

End				