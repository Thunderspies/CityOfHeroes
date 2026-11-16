#############################################################
## EmbersHitSootandCinders.fx
#############################################################

FxInfo
LifeSpan	75

#############################################################

Input
	Inpname	Hips
End

################################################################

Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	chest
		Part	:sparkA1.part
		Part	:sparkB1.part

		LifeSpan	6
	End

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part	:MartialArtsHit.part
		Part	:MartialArtsHit1.part
		Part	:MartialArtsHitRaysSmallest.part
		Sound cinders3 70 70 .4
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part	CustomizeablePowers\FireControl\Subtractive\MAHitStar01Sml.part
		Part	CustomizeablePowers\FireControl\Subtractive\MAHitStar02Sml.part
		Part	CustomizeablePowers\FireControl\Subtractive\MAHitStar03Sml.part
		Part	CustomizeablePowers\FireControl\Subtractive\MAHitRays01Sml.part


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
		ChildFX :FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


End