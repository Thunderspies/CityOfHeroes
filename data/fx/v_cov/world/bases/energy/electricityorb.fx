#########################################################
##	forceBubble
########################################################
FxInfo

########################################################
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
		Sound energyorb_loop 110 110 .58
	End
End




Condition
	On 	Time
	Time 	0

	Event
		
		Type	PositOnly
		At	Hips
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:ElectricityCageRingBrighta.part
		Sound electricityorb2_loop 110 110 .68
				
	End

End

Condition
	On 	Time
	Time 	15

	Event
		
		Type	PositOnly
		At	Hips
		BHVR	Behaviors/GenericParticleFade.bhvr
		Part1	:ElectricityCageArchs2a.part
		Part2	:ElectricityCageGlowa.part
		Part3	:ElectricityCageArchsa.part
	
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
