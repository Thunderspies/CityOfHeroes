#############################################################
## ToxicGas_Inward.fx
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
		Sound 	Sewer_ToxicGasInward_01 100 100 .8
		lifespan 1
	End
End

Condition
	On	Time
	Time	0

	Event
		Ename	GasIn
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 3 0
		End
		Part	:ToxicGas_In.part
		Part	:ToxicGas_InHilight.part
		#Lifespan 60
	End
End
#############################################################
End