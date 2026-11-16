#############################################################
## Thrown Confetti FX
#############################################################

FxInfo

Lifespan 30

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Mystic
		LifeSpan 	5
		Part1	:ConfettiSparkles.part
	End
End

## LARGE CONFETTI - WHITE ###########################################################

Condition
	On 	Time
	Time 	0
	Repeat 		12
	RepeatJitter	4

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 255 255
		End
		geom	FX_ConfettiHeart
		geom	FX_ConfettiDiamond
	End
End

### SMALL CONFETTI - WHITE ###########################################################

Condition
	On 	Time
	Time 	1
	Repeat 		6
	RepeatJitter	2

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 255 255
			Scale		0.0625 0.0625 0.0625
		End
		geom	FX_ConfettiCircle
		geom	FX_ConfettiRectangle
	End
End

## LARGE CONFETTI - PINK ###########################################################

Condition
	On 	Time
	Time 	2
	Repeat 		12
	RepeatJitter	4

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 192 192
		End
		geom	FX_ConfettiHeart
	End
End

## SMALL CONFETTI - PINK ###########################################################

Condition
	On 	Time
	Time 	3
	Repeat 		6
	RepeatJitter	2

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 192 192
			Scale		0.0625 0.0625 0.0625
		End
		geom	FX_ConfettiCircle
		geom	FX_ConfettiRectangle
	End
End

## LARGE CONFETTI - GOLD ###########################################################

Condition
	On 	Time
	Time 	4
	Repeat 		12
	RepeatJitter	4

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 192 160
		End
		geom	FX_ConfettiBell
		geom	FX_ConfettiDiamond
	End
End

## SMALL CONFETTI - GOLD ###########################################################

Condition
	On 	Time
	Time 	5
	Repeat 		6
	RepeatJitter	2

	Event
		Type	Start
		At	Mystic
		Bhvr	:Confetti.bhvr
		BhvrOverride
			StartColor	255 240 192
			Scale		0.0625 0.0625 0.0625
		End
		geom	FX_ConfettiCircle
		geom	FX_ConfettiRectangle
	End
End

#############################################################

End