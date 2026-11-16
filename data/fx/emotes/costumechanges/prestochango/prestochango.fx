#############################################################
## PrestoChango.fx
#############################################################

FxInfo

LifeSpan 120

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound Emote_Presto_01 80 80 0.8
	End
End

#############################################################


Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Part	POWERS\ItemsAndTempPowers\MidnightVisage\Flash_Core.part
		Part	POWERS\ItemsAndTempPowers\MidnightVisage\Flash_Glow.part
	End
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Root
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Smoke01.part
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Smoke02.part
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Smoke_Wispy01.part
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Smoke_Wispy02.part
		Lifespan 3
	End

	Event
		Type	Local
		At	Root
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Sparkles01.part
		Lifespan 60
	End
End

#############################################################

Condition
	On	Time
	Time	1

	Event
		Type	Local
		At	Neck
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	Hips
		Lifespan 2
	End

	Event
		Type	Local
		At	Hair
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	Neck
		Lifespan 2
	End

	Event
		Type	Local
		At	ULegL
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	LLegL
		Lifespan 2
	End

	Event
		Type	Local
		At	ULegR
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	LLegR
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmL
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	LArmL
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmR
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	LArmR
		Lifespan 2
	End
End

Condition
	On	Time
	Time	2

	Event
		Type	Local
		At	LLegL
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	FootL
		Lifespan 2
	End

	Event
		Type	Local
		At	LLegR
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	FootR
		Lifespan 2
	End

	Event
		Type	Local
		At	UArmL
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	WepL
		Lifespan 2
	End

	Event
		Type	Local
		At	LArmR
		part	POWERS\ItemsAndTempPowers\MidnightVisage\Body_Smoke01.part
		POther	WepR
		Lifespan 2
	End
End

#############################################################

End