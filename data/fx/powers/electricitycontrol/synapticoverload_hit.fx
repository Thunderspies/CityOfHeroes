#############################################################
## SynapticOverload_Hit.fx
#############################################################

FxInfo

#############################################################

Input
	Inpname		Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	HandGlow1
		Type	Local
		At	Head
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Ring_Burst.part
		Part	:Body_Sparks.part
		Part	:Hit_Flash_Small.part
		Lifespan 5
		Sound	elecfry 80 70 .8
	End

	Event
		EName	HandGlow1
		Type	Local
		At	Head
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Head_Ring_Continuing.part
	End
End

#############################################################

End
