#############################################################
## EmbersHitSootandCinders.fx
#############################################################

FxInfo
LifeSpan	75

#############################################################

Input
	InpName	Hips
End

################################################################


Condition
	On	Time
	Time	0

	Event
		EName	Pivot
		Type	local
		At	chest
		#Part1	:ConsumeMediumfire_flame_A.part
		Part	:sparkA1.part
		Part	:sparkB1.part

		LifeSpan	6
	End

	Event
		EName 	Mallet
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MartialArtsHit.part
		Part2	Powers/MartialArts/MartialArtsHit1.part
		#Part3	Powers/MartialArts/MartialArtsHit2.part
		Part4	Powers/MartialArts/MartialArtsHitRaysSmallest.part
		Sound cinders3 70 70 .4
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01Sml.part
		Part2	Powers/MartialArts/MAHitStar02Sml.part
		Part3	Powers/MartialArts/MAHitStar03Sml.part
		#Part4	Powers/MartialArts/MAHitGlow01Sml.part
		Part5	Powers/MartialArts/MAHitRays01Sml.part


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
		ChildFX V_COV\PhysicsEnabled\FireBurstScatter.fx
		Lifespan 70
		LifespanJitter 	20
	End
End


End