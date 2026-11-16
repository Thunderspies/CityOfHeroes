#########################################################
## Staff of the M.A.G.I. - Ice Burst (HIT)
#########################################################

FxInfo

LifeSpan	200

## HIT FX ###############################################################

Condition
	On	Time
	Time	0

	Event
		EName	IceShards
		Type	Local
		At	Chest
		Part1	POWERS\ColdControl\IceBoltMist.part
		Part2	POWERS\ColdControl\ChillTouchSnow.part
		Part3   POWERS\ColdControl\ChillingTouchFlash.part
		lifespan	20
	End
End

#################################################################

End