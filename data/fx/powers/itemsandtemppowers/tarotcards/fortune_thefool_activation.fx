#############################################################
## Fortune_TheFool_Activation.fx
#############################################################

FxInfo

Lifespan 300

## ACTIVATION FX ###########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		ChildFX	:Child_WheelOfFortuneSigil.fx
		Sound Tarot_Fool_01 30 30 0.8
	End
End

#############################################################

Condition
	On	Time
	Time	35

	Event
		EName	BigCard
		Type	PositOnly
		At	Chest
		Part	:Continuing_InitialTarotCard_TheFool.part
		Part	:Continuing_InitialTarotCard_Flare1.part
		Part	:Continuing_InitialTarotCard_Flare2.part
		Lifespan 10
	End
End

#############################################################

End