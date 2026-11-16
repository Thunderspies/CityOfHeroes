#########################################################
##	RadiationAura
FxInfo

Input  
	InpName	Head
End

########################################################

LifeSpan 30

Condition
	On	Time
	Time	0

	Event
		
		Type	Posit
		At	Head
		part1	:RadiationElectronsHit.part
		part2	:ChockingCloudHitDotsHit.part
		Part3	:ChockingCloudHitRings.part
		part4	:ChockingCloudHit.part
		part5	:ChockingCloudHit2.part
		PMagnet	Head
		
	End


End

End				