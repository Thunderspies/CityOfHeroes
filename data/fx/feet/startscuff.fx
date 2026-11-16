#########################################################
## 	Start Scuff
#########################################################

FxInfo

Input
	InpName FootL
End

LifeSpan 1
Condition
	Event
		Type	Local
		At	FootL
		SoundNoRepeat 3
		Sound FootScuff1 50 50 .07
		Sound FootScuff2 50 50 .07
		Sound FootScuff3 50 50 .07
		Sound FootScuff4 50 50 .07
		Sound FootScuff5 50 50 .07
	End
End
