#############################################################
## EmbersHitSootandCinders.fx
#############################################################

FxInfo
LifeSpan	75

#############################################################

Input
	InpName	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	chest
		Part	CustomizeablePowers\FireControl\sparkA1.part
		Part	CustomizeablePowers\FireControl\sparkB1.part

		LifeSpan	6
	End

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part	CustomizeablePowers\FireControl\MartialArtsHit.part
		Part	CustomizeablePowers\FireControl\MartialArtsHit1.part
		Part	CustomizeablePowers\FireControl\MartialArtsHitRaysSmallest.part
		Sound cinders3 70 70 .4
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part	CustomizeablePowers\FireControl\MAHitStar01Sml.part
  	Part	CustomizeablePowers\FireControl\MAHitStar02Sml.part
  	Part	CustomizeablePowers\FireControl\MAHitStar03Sml.part
		Part	CustomizeablePowers\FireControl\MAHitRays01Sml.part


	End

End

Condition
	On 	time
	Time 	0
	Repeat  5

	Event
		EName 	Sparkz
		Type 	Local
		At 	hips
#		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


End