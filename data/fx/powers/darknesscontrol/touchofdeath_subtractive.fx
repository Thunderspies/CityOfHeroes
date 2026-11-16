#########################################################
##	Fire_Ball
##

#########################################################

FxInfo
LifeSpan	55

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On	Time
	Time	15

	Event
		Type	Local
		At	UarmL
		Sound DClifedrain4 70 70 .86
	End
End



Condition
	On 	Time
	Time 	23

	Event

		ENAME	skull
		Type	local
		At	WepL
		Part	:TouchOfFearHandTendril.part
		LifeSpan	31
	End

End

Condition
	On 	Time
	Time 	13

	Event
		EName 	Hand1
		Type	Local
		At	WepL

		Part	:FearRing_subtractive.Part
		LifeSpan	55
	End

End

Condition
	On 	Time
	Time 	33

	Event
		EName 	Hand1
		Type	Local
		At	WepL

		Part	:FearSkullrays.part

		LifeSpan	25
	End

End

Condition
	On 	Time
	Time 	18

	Event
		EName 	Hand1
		Type	Local
		At	WepL

		Part	:FearRing2_Subtractive.Part
		LifeSpan	55
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand3
		Type	Local
		At	UarmL

		Part3	:DarkBlastHitTendril.part
		Part1	:DarkHandsA.part
		Part2	:DarkHandsB.part
		Part3	:SmokeHands.part
		Part	:SmokeHands2_Subtractive.part
		Part4	:SmokeHands2.part

		Part5	:DarkHandsEmberb.part

		Part	:DarkHandsB.part
		Part	:DarkHandsa.part

		PMagnet	LarmL
		LifeSpan	50
	End

End

Condition
	On 	Time
	Time 	5

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		Part3	:DarkBlastHitTendril.part
		Part1	:DarkHands.part
		Part2	:DarkHands2.part
		Part3	:SmokeHands.part
		Part	:SmokeHands2_Subtractive.part
		Part4	:SmokeHands2.part

		Part5	:DarkHandsEmberb.part

		Part	:DarkHandsB.part
		Part	:DarkHandsa.part

		PMagnet	WepL
		LifeSpan	50
	End

End


End


