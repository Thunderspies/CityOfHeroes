#############################################################
## ToxicShock_Tell.fx
#############################################################

FxInfo
//Lifespan 90
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_ToxicShock_Tell_01 100 100 .8
	End

	Event
		Type	PositOnly
		At	Root
		Part	:Toxic_OuterRing_Alpha.part
		Part	:Toxic_OuterRing_Alpha_ground.part
	eND
End
#############################################################
End