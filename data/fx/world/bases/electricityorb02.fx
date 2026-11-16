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
		Part1	:ElectricityCageRingBrighta.part
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
		Part1	:ElectricityCageArchs2b.part
		Part2	:ElectricityCageGlowb.part
		Part3	:ElectricityCageArchsb.part
	
	End

	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs2
		bhvr	behaviors/cagespin4.bhvr
	End



End

Condition
	On 	Time
	Time 	16	
	Event
		
		Type	PositOnly
		At	Hips
		Geom	Cagearchs
		bhvr	behaviors/cagespin3.bhvr
	End


End

End
