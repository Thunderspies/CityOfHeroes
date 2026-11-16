#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End

Lifespan 100

##################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepR
		Sound miasmaburst 100 100 .8
		End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	WepL
		Sound miasma_loop 100 100 .5
		End
End



Condition
	On	Time
	Time	0
		
	Event	

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		LifeSpan	90

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	CustomizeablePowers\DarkMiasma\DarkHand.part
		Part3	CustomizeablePowers\DarkMiasma\DarkHandsEmber.part
		LifeSpan	90
	End

End

End				