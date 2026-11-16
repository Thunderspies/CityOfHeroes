#########################################################
##	iceweapon
########################################################
FxInfo

Input
	Inpname	Chest
End

Lifespan 10

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	local
		At	Chest
		#Part	Powers/MartialArts/MartialArtsHit.part
		Part	Powers/MartialArts/MartialArtsEmberHit1.part
		Part	Powers/MartialArts/MartialArtsHitEmber2.part
		Part	Powers/MartialArts/HitRaysSmall.part
		Part	Powers/MartialArts/HitRaysLarge.part
		Part	:CoreFlash.part
		Part	:CoreRings.part
		Part	:Embers.part
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		Part4	Powers/MartialArts/MAHitGlow.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part
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


End
