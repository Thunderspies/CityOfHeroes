#############################################################
## HEADER
#############################################################

FxInfo

ClampMinScale 0.875 0.875 0.875
ClampMaxScale 1.125 1.125 1.125

Lifespan 10

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	Chest
		Part1	:Hit.part
		Part2	:Hit1.part
		Part3	:Hit2.part
		Part4	:HitRaysSmall.part
	End

	Event
		Type	local
		At	Chest
		Part1	:HitStar01Sml.part
		Part2	:HitStar02Sml.part
		Part3	:HitStar03Sml.part
		Part4	:HitGlow01Sml.part
		Part5	:HitRays01Sml.part
		Sound pp13 100.0 100.0 .7
	End
End

#############################################################

End