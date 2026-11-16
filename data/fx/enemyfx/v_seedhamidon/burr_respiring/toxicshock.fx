#############################################################
## ToxicShock.fx
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
		Sound 	HamidonSeed_ToxicShock_01 200 100 .8
	End

	Event
		Type	PositOnly
		At	Root
		Part	:ToxicShock_Attack_Alpha.part
		Part	:ToxicShock_Attack_Alpha_out.part
		Part	:ToxicShock_Attack_Mult.part
		Part	:ToxicShock_Attack_Mult_out.part
	eND
End
#############################################################
End