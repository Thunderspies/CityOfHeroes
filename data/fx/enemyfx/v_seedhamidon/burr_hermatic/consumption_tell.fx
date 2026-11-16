#############################################################
## Consumption.fx
#############################################################

FxInfo
Lifespan 240
#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		Sound 	HamidonSeed_Consumption_Tell_01 100 100 .8
	End

	Event
		Type	PositOnly
		At	Root
		Part	:Toxic_OuterRing_Alpha_ground.part
		Part	:NegativeCenter.part
		Lifespan 200
	End
End
#############################################################
End