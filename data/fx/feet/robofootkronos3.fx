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
		Type Local
		At origin
		Sound robofoot3 400 400 .44
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
