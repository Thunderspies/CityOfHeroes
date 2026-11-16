#############################################################
## PsychicLash_Windup.fx
#############################################################

FxInfo
Lifespan 38

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Hair
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		Lifespan 4
	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Local
		At	Chest
		Part	:Ring_Windup.part
		Part	:Electricity_Large.part
		Part	:PsychicBattery_Smoke.part
		Part	:Glow_Large.part
		POther 	Head
		Lifespan 4
	End
End

Condition
	On	Time
	Time	6

	Event
		Type	Local
		At	LArmR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	UArmR
		Lifespan 4
	End

	Event
		Type	Local
		At	LArmL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	UArmL
		Lifespan 4
	End

	Event
		Type	Local
		At	Hips
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	Chest
		Lifespan 4
	End
End

Condition
	On	Time
	Time	9

	Event
		Type	Local
		At	WepR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	LArmR
		Lifespan 4
	End

	Event
		Type	Local
		At	WepL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	LArmL
		Lifespan 4
	End

	Event
		Type	Local
		At	LLegR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	ULegR
		Lifespan 4
	End


	Event
		Type	Local
		At	LLegL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	ULegL
		Lifespan 4
	End
End

Condition
	On	Time
	Time	12

	Event
		Type	Local
		At	FootR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	LLegR
		Lifespan 4
	End

	Event
		Type	Local
		At	FootL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		POther 	LLegL
		Lifespan 4
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	WepR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		Lifespan 4
	End

	Event
		Type	Local
		At	WepL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		Lifespan 4
	End

	Event
		Type	Local
		At	FootR
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		Lifespan 4
	End

	Event
		Type	Local
		At	FootL
		Part	:PsychicBattery_Electricity.part
		Part	:PsychicBattery_Smoke.part
		Part	:PsychicBattery_Glow.part
		Lifespan 4
	End
End

#############################################################

End