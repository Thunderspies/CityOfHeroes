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
		Sound vfoot1 95 95 .29
	End

End

Condition
	Event
		Type	Start
		At	FootR
		part	ENEMYFX\Malta\Titan_EyeFX\TitanStepKronos01.part
		Bhvr	Behaviors\KronosShake01.bhvr
	End

End


End
