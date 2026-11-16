#########################################################
##	template

FxInfo


#########################################################

Condition
	On Time
	Time 0

	Event
		EName	Pow
		Type 	Local
		At	Chest

		bhvr	behaviors\FX_SnakeMen_HitScale.bhvr
		Part3	Powers/Claws/ClawEmbers.part
		Part4	Powers/Claws/ClawEmbersBroad.part
		Part5	Powers/Claws/ClawsStar.part
		Sound Swordhit3 80.0 80.0 .8
	End

	Event
		EName 	Mallet
		Type	local
		At	Chest

		bhvr	behaviors\FX_SnakeMen_HitScale.bhvr
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	powers/MedievalWeapons/SwordHitRaysLarge.part
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest

		bhvr	behaviors\FX_SnakeMen_HitScale.bhvr
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
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



	Event
		Type	local
		At	Chest

		bhvr	behaviors\FX_SnakeMen_HitScale.bhvr
		Part	powers/MedievalWeapons/ExplodingSteaks.part
#		Part	powers/MedievalWeapons/RaysUp2.part


	End

End

Condition
	On Time
	Time 2

	Event
		EName	Pow
		Type 	Destroy

	End

End

Condition
	On Time
	Time 6

	Event
		EName	all
		Type 	Destroy

	End

End



End

