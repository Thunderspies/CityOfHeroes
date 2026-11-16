#############################################################
## Hit_PsionicDamage.fx
#############################################################

FxInfo
Lifespan 30

#############################################################

Input
	Inpname	Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:InfuseLight2.part
		Part2	:Infuseglow.part
		Part3	:InfuseRings.part
		Part3	:Defuseglow.part
		Part2	:HitRaysBig.part
		Part4	:HitStreaksBig.part
		Sound Infuse4 80 80 .6
	End

	Event
		Type	local
		At	Head
		BHVR	Behaviors\GenericParticleFade.bhvr
		Part1	:PsionicBubblesHit.part
		Part4	:PercentSign.part
		Part5	:Asterick.part
		Part4	:QuestionMark.part
		Part5	:NumberSign.part
		Lifespan 25
	End
End

#############################################################

End