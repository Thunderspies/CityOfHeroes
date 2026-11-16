#############################################################
## MartialArtsHitsDarkness.fx
#############################################################

FxInfo
Lifespan 10

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		EName 	Mallet
		Type	local
		At	Chest

		Part1	:MartialArtsHit.part
		Part2	:MartialArtsHit1Black.part
		Part3	:MartialArtsHit2Black.part
		Part5	:MartialArtsHitRaysDark.part
		Sound Shadowpunch3 60 60 .76
	End

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	:MAHitStar01Sml.part
		Part2	:MAHitStar02Sml.part
		Part3	:MAHitStar03Sml.part
		Part4	:MAHitGlow01Sml.part
		Part5	:MAHitRays01Sml.part
		Sound Hit3 100.0 100.0 .8

	End
End


End
