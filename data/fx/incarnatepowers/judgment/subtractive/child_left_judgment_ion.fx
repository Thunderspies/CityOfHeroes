#############################################################
## CHILD_RIGHT_Judgment_Ion.fx
#############################################################

FxInfo
LifeSpan	80

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0.0 50.0 0.0
		End
		Part	:ChargedBolt01.part
		Part	:ChargedBolt02.part
		Part	:ChargedBolt03.part
		Part	:Hand_Bolt_Blur_Small.part
		LookAt	WepL
		Pother	WepL
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	StartPositOnly
		At	WepL
		Part	:Ion_Flash.part
		Part	:Ion_Flash_Electric.part
		Part	:Hand_Sparks.part
		Part	:Hand_Sparks_Long.part

		Lifespan	3
	End
End

#############################################################

End