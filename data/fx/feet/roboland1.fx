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
		Sound vland1 50 50 .44
	End

End

Condition
	Event
		Type	Local
		At	FootR
		part	ENEMYFX\Malta\Titan_EyeFX\TitanStep01.part
	End

End

Condition
	Event
		Type	Local
		At	FootL
		part	ENEMYFX\Malta\Titan_EyeFX\TitanStep01.part
	End

End

End
