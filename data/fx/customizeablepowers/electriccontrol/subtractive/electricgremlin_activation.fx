#############################################################
## ElectricControl_Activation.fx
#############################################################

FxInfo
Lifespan 80

#############################################################


Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy
		Type	local
		At	WepR
		Sound EC_Gremlins_Activate_01 100 100 .8
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
	#	Sound	cagebolt2 80 30 .8
	End

	Event
		EName 	Thingy1
		Type	local
		At	WepL
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part	:Hand_Ring.part
		Part	:Hand_Sparks.part
	End
End

#############################################################

End