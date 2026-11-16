#########################################################
##	template

FxInfo
LifeSpan	105

Input
	Inpname chest
End


Condition

	Event
	Type 	Local
	At	chest
	Sound RiktiSword2hit 80.0 80.0 .7

	End

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	Powers/MartialArts/MartialArtsHitRaysSmall.part
		LifeSpan	10
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
		LifeSpan	10

	End

End

End



