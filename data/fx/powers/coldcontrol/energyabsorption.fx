#########################################################
##	chill touch hit
FxInfo
Input  
	InpName Root
End

Input  
	InpName	head
End

Input  
	InpName	origin
End

LifeSpan	90

##################################

Condition
	On 	Time
	Time 	19

	Event	
		Type	Local
		At 	origin
		Sound buildup 100 100 .78

	End

End

Condition
	On 	Time
	Time 	0

	Event	
		Type	Local
		At 	origin
		Sound coldblast2 100 100 .8

	End

End

Condition
	On 	Time
	Time 	10

	Event	
		Type	Local
		At 	origin
		Sound coldblast2 100 100 .7

	End

End

Condition
	On	Time
	Time	57
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	head
		Part3	:O2MistUpwards1.part
		Part4	:O2MistUpwards2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	50
	End
End


Condition
	On	Time
	Time	65	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		PMAGNET	head
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part1	:O2RingUp.part
		LifeSpan	20
	End
		
End


Condition
	On	Time
	Time	47
	
	Event	
		Ename   targetchill2
		Type	Local
		At	Root
		PMAGNET	head
		Part4	:O2Snow.part
		Part3	:SnowUpwards1.part
		LifeSpan	5
		bhvr	Behaviors/GenericParticleFade.bhvr
	End

	Event	
		Ename   targetchill2
		Type	Local
		At	Root
		PMAGNET	head
		Part1	:O2Mist.part
		Part2	:O2Mist2.part
		bhvr	Behaviors/GenericParticleFade.bhvr
		LifeSpan	15
	End

	Event	
		Ename   targetchill2
		Type	Local
		At	Root
		PMAGNET	head
		
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part5	:O2SnowFlakes.part
		LifeSpan	30
	End
	
	Event	
		Ename   targetchill
		Type	Local
		At	Root
		bhvr	Behaviors/GenericParticleFade.bhvr
		Part2	:O2BoostMistDownFlat.part
		LifeSpan	5		
	End

	Event	
		Ename   geom
		Type	Local
		At	Root
		geom	IceAbsorption
		bhvr	behaviors/Icespin.bhvr
		LifeSpan	15
	End

End


Condition
	On	Time
	Time	0

	Event	
		Ename   ringMist
		Type	Local
		At	Root
		
		Part5	:O2Ring.part
		Part1	:coldMistRingFlat.part
		Part3	:coldSnowRingFlat.part
		Part2	:coldSnow.part
		bhvr	Behaviors/GenericParticleFade.bhvr

		LifeSpan	17
	End

	
End


End				
