#############################################################
## "The Dragon" - Breath of Fire
#############################################################

FxInfo

Lifespan	150

## SOUNDS ###########################################################

Condition
	On 	Time
	Time 	35

	Event
		Type	Local 
		At	Head
		Sound Firehand_loop 100.0 100.0 .45
		Sound Ignite2 100.0 100.0 .8
		LifeSpan	60
	End
End

## FLAME ###########################################################

Condition
	On 	Time
	Time 	30

	Event
		EName	MouthOffset
		Type	Local
		At	Head
		bhvr	behaviors/scale1a.bhvr	
		BhvrOverride
			PyrRotate	-85 0 0
			PositionOffset	0 -0.25 1.25
		End
		Lifespan	80
	End

	Event
		EName	GasStream
		Type	Local
		At	MouthOffset
		Part1	POWERS\FireControl\FireSpit.part
		Part2	POWERS\FireControl\FireSpecksSpit.part
		Part3	POWERS\FireControl\FireSpecksFalling.part
		Part4	POWERS\FireControl\FireSpit2.part
		Part5	POWERS\FireControl\FireSpitStart.part
		Sound arcoflame 100 100 .88
		Lifespan	80
	End
End

#############################################################

End				