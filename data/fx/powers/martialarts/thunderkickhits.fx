#############################################################
## ThunderKickHits.fx
#############################################################

FxInfo
Lifespan 100

#########################################################

Input
	Inpname	Hips
End

#########################################################


Condition
	On 	Time
	Time 	2

	Event
		EName 	HitFX
		Type	local
		At	Chest
		Part1	:MAHitStar01.part
		Part2	:MAHitStar02.part
		Part3	:MAHitStar03.part
		Part4	:MAHitGlow01.part
		Part5	:MAHitRays01.part
		Sound Thunderhit 60.0 60.0 .8

	End

End


End
