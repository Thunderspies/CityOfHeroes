#############################################################
## ToxicGas_Outward.fx
#############################################################

FxInfo
lifespan 500
#############################################################

Condition
	On 	Time
	Time	0
	Event
		Type	Local
		At	Root
		Sound 	Sewer_ToxicGasOutward_01 100 100 .8
		lifespan 1
	End
End

Condition
	On	Time
	Time	5

	Event
		Ename	GasIn
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 2 0
		End
		Part	:ToxicGas_Flash.part
		Part	:ToxicGas_Out.part
		Part	:ToxicGas_OutHilight.part
		Part	:ToxicGas_Lingering.part
		Part	:ToxicGas_LingeringHilight.part
		Lifespan 40
	End
End
#############################################################
End