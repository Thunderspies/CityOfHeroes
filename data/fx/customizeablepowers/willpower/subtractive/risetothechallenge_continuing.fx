#########################################################
##  Willpower - Continuing effect
#########################################################

FxInfo

#########################################################

Condition
	On	Cycle
	Time	40

	Event
		Ename	Hearbeat
		Type	Local
		At	Origin
		ChildFX	CustomizeablePowers\ContinuingConditionalFX\Subtractive\WillPower_Continuing_Heartbeat.fx
		Lifespan	60
	End
End

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		sound	heart_loop 50 40 .21
		Bhvr	Behaviors\soundFade3.bhvr
		Lifespan	60
	End
End

Condition
	On	Time
	Time	31

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part5 :Glow01.part
		Part5 :ERing2.part
		Part1 :Cloud01.part
		Part2 :Cloud02.part
		Part3 :ERing.part
		Part4 :Cloud03.part
	End
End

Condition
	On	Time
	Time	44

	Event
		Ename	Ring
		Type	Local
		At	Hips
		Part5 :ERing2.part
		Part1 :Cloud01.part
		Part2 :Cloud02.part
		Part3 :ERing.part
		Part4 :Cloud03.part
	End
End



## RICHOCETS #######################################################

Condition
	On 	cycle
	Time 	17
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegL
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LlegR
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LarmR
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	20
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	LarmL
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	UarmL
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.125

	Event
		EName	3
		Type	Local
		At	UarmR
		part	:FortitudeRaysSmall2.part
		part	:FortitudeRing2.part
		lifespan	25
	End
End

#########################################################


End