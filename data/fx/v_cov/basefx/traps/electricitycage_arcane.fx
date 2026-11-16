#########################################################
##	forceBubble
########################################################
FxInfo

########################################################





Condition
	On 	Time
	Time 	0

	Event
		
		Type	PositOnly
		At	Hips
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:ElectricityCageRing.part
		Part1	:ElectricityCageRingBright.part
		Sound eleccage_loop 100 100 .8
				
	End

End

Condition
	On 	Time
	Time 	15

	Event
		
		Type	PositOnly
		At	Hips
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:ElectricityCageArchs2.part
		Part2	:ElectricityCageGlow.part
		Part3	:ElectricityCageArchs.part
	
	End


	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs2
		bhvr	:cagespin2.bhvr
	End

	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs
		bhvr	:cagespin.bhvr
	End


End

Condition
	On 	Time
	Time 	16	
	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs
		bhvr	:cagespin.bhvr
	End


	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs2
		bhvr	:cagespin2.bhvr
	End



End

End
