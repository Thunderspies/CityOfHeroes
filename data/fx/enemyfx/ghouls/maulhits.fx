#############################################################
## MaulHits.fx
#############################################################

FxInfo
LifeSpan	71

##############################################################################

Input
	Inpname Hips
End

##############################################################################


Condition
	On Time
	Time 1

	Event
		EName 	HitFX2
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml2.part
		Part3	Powers/MartialArts/MAHitStar03Sml2.part
		Part4	Powers/MartialArts/MAHitGlow01Sml2.part
		Part5	Powers/MartialArts/MAHitRays01Sml2.part
		Part	Powers/MartialArts/MartialArtsHitRaysDark.part
		LifeSpan	10
	End

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
	End


End

#############################################################

Condition
	On cycle
	Time 5
	Chance	.9

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml2.part
		Part4	Powers/MartialArts/MAHitGlow01Sml2.part
		Part5	Powers/MartialArts/MAHitRays01Sml2.part
		Part	Powers/MartialArts/MartialArtsHitRaysDark.part
		LifeSpan	1
	End

	Event
		Type	Local
		At	Chest
		SoundNoRepeat 3
		Sound 	N_Hit_01 100 100 .8
		Sound 	N_Hit_02 100 100 .8
		Sound 	N_Hit_03 100 100 .8
		Sound 	N_Hit_04 100 100 .8
		Sound 	N_Hit_05 100 100 .8
		Sound 	N_Hit_06 100 100 .8
		lifespan 1
	End

End

####################################################################


Condition
	On Time
	Time 71

	Event
		EName 	Mallet2
		Type	local
		At	Chest

		Part	:MaulHitRing.Part
		Part	:MaulBalls.part

		LifeSpan	10
	End

	Event
		EName 	HitFX2
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml2.part
		Part3	Powers/MartialArts/MAHitStar03Sml2.part
		Part4	Powers/MartialArts/MAHitGlow01Sml2.part
		Part5	Powers/MartialArts/MAHitRays01Sml2.part
		Part	Powers/MartialArts/MartialArtsHitRaysDark.part
		Sound Shadowpunch3 60 60 .8
		Sound Hit6 60 60 .8


		LifeSpan	10

	End

End

End
