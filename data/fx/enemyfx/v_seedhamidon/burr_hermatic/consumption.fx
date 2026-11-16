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
		Sound 	HamidonSeed_Consumption_01 200 100 .8
	End

	Event
		Type	local
		At	Root
		Part	:Consumption_Attack_Alpha.part
		Part	:Consumption_Attack_Alpha_out.part
		Part	:Consumption_Attack_Mult.part
		Part	:Consumption_Attack_Mult_out.part
		Lifespan 200
	End
End
#############################################################
End