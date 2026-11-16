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
		Part	:Hand_Electricity_Arcs_Consistant.part
		Part	:Hand_Electricity_Arcs_Consistant2.part
		Part	:Hand_Glow.part
		Part3	:Hand_Ring.part
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