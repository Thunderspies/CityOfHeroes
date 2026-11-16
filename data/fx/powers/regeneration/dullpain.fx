#############################################################
## DullPain.fx
#############################################################

FxInfo

#####################################################################################

Condition
	On	Time
	Time	5

	Event
		Type	Local
		At	Hips
		Part2	Powers\Regeneration\GreenMist.part
		Sound regen8c 60 60 0.76
		LifeSpan 60
	End
End

Condition
	On	Time
	Time	58

	Event
		Type	Local
		At	Hips
		Part1	POWERS\Regeneration\Ring03.part
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	UlegL
		part1	Powers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		Type	Local
		At	UlegR
		part1	Powers\Regeneration\RegenBodyGlows2.part
		PMagnet	Hips
	End

	Event
		Type	Local
		At	LlegL
		part1	Powers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGL
	End

	Event
		Type	Local
		At	LlegR
		part1	Powers\Regeneration\RegenBodyGlows2.part
		PMagnet	ULEGR
	End

	Event
		Type	Local
		At	FootL
		part1	Powers\Regeneration\RegenBodyGlows3.part
		PMagnet	LLEGL
	End

	Event
		Type	Local
		At	FootR
		part1	Powers\Regeneration\RegenBodyGlows3.part
		PMagnet	LLEGR
	End

	Event
		Type	Local
		At	FootR
		part1	Powers\Regeneration\RegenBodyGlows.part
		PMagnet LlegR
	End

	Event
		Type	Local
		At	ULEGL
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	LLEGL
		part1	Powers\Regeneration\RegenBodyGlows.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	FootL
		part1	Powers\Regeneration\RegenBodyGlows.part
		PMagnet LlegL
	End

	Event
		Type	Local
		At	ULEGR
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	LLEGR
		part1	Powers\Regeneration\RegenBodyGlows.part
		PMagnet UlegL
	End

	Event
		Type	Local
		At	Head
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	UArmL
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	LArmL
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	WepL
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	UArmR
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	LArmR
		part1	Powers\Regeneration\RegenBodyGlows.part
	End

	Event
		Type	Local
		At	WepR
		part1	Powers\Regeneration\RegenBodyGlows.part
	End
End

#############################################################

Condition
	On 	cycle
	Time 	20
	Chance	0.2

	Event
		Type	Local
		At	LarmL
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

Condition
	On 	cycle
	Time 	19
	Chance	0.2

	Event
		Type	Local
		At	LarmR
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

Condition
	On 	cycle
	Time 	21
	Chance	0.2

	Event
		Type	Local
		At	UarmL
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

Condition
	On 	cycle
	Time 	22
	Chance	0.2

	Event
		Type	Local
		At	UarmR
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

Condition
	On 	cycle
	Time 	17
	Chance	0.2

	Event
		Type	Local
		At	LlegL
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

Condition
	On 	cycle
	Time 	18
	Chance	0.2

	Event
		Type	Local
		At	LlegR
		part1	Powers\Regeneration\FortitudeRaysSmall2.part
		part2	Powers\Regeneration\FortitudeRing2.part
		part3	Powers\Regeneration\FortitudeRing2.part
		part4	Powers\Regeneration\FortitudeDots2.part
		LifeSpan 25
	End
End

#############################################################

End