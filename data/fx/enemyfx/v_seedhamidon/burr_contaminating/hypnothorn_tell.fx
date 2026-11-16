#############################################################
## HypnoThorn_Tell.fx
#############################################################

FxInfo
Lifespan 150
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_HypnoThorn_Tell_01 100 100 .8
	End

	Event
		Type	local
		At	Root
		Part	:Toxic_OuterRing_Alpha_ground.part
		Lifespan 80
	eND
End
#############################################################
End