#########################################################
##	MH
## 	Female Left Foot

FxInfo

Input
	InpName Origin
End

LifeSpan 5

Condition
	Event
		Type	Local
		At	origin
		Sound vfoot2 95 95 .28
	End

End

Condition
	Event
		Type	Start
		At	FootL
		part	ENEMYFX\Malta\Titan_EyeFX\TitanStepKronos01.part
		Bhvr	Behaviors\KronosShake01.bhvr
	End

End

End
